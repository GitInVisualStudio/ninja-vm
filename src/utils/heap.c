#include "heap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEFT 0
#define RIGHT 1

int _allocations = 0;

// defined in heap.h
Heap heap = { 
    .data = NULL,
    .first_free = NULL,
    .side = 0,
    .size = 0,
    .debug = false,
    .purge = false
};

void heap_init(realloc_callback set_used, int sizeKiB) {
    const int bytes = 1024 * sizeKiB;
    heap.data = (char*)malloc(bytes);
    heap.first_free = heap.data;
    heap.size = bytes;
    heap.side = LEFT;
    heap.set_used = set_used;
}

void heap_delete() {
    __heap_collect();
    if (heap.data)
        free(heap.data);
    heap.size = 0;
    heap.first_free = 0;
}

inline bool __heap_full(unsigned int size) {
    return (heap.side == RIGHT && heap.first_free + size + sizeof(Alloc) >= heap.data + heap.size) 
        || (heap.side == LEFT && heap.first_free + size + sizeof(Alloc) >= heap.data + heap.size / 2);
}

void* heap_alloc(unsigned int size) {
    if (__heap_full(size)) {
        __heap_collect();
        // not able to allocate the given amounts of bytes
        if (__heap_full(size)) 
            return NULL;
    }
    Alloc* newAlloc = (Alloc*)heap.first_free;
    newAlloc->size = size;
    newAlloc->data = heap.first_free + sizeof(Alloc);
    heap.first_free += size + sizeof(Alloc);
    memset(newAlloc->data, 0, size);
    _allocations++;
    return newAlloc;
}

void __heap_collect() {
    int prevAllocs = _allocations;
    int data = heap.first_free - (heap.data + (heap.side == LEFT ? 0 : heap.size / 2));
    _allocations = 0;

    /**
     * we first have to update the heaps side so we can tell which ptrs are already realloced
     */
    heap.side = !heap.side;
    heap.first_free = heap.side == RIGHT ? (heap.data + heap.size / 2) : heap.data;
    if (heap.set_used)
        heap.set_used();

    if (heap.debug) {
        int newData = heap.first_free - (heap.data + (heap.side == LEFT ? 0 : heap.size / 2));
        printf("Garbage Collector:\n");
        printf("\t%i objects (%i bytes) allocated since last collection\n", prevAllocs, data);
        printf("\t%i objects (%i bytes) copied during this collection\n", _allocations, newData);
        printf("\t%i of %i bytes free after this collection\n", heap.size / 2 - newData, heap.size / 2);
    }

    if (heap.purge)
        memset(heap.data + (heap.side == RIGHT ? 0 : heap.size / 2), 0, heap.size / 2);
}

inline bool heap_alloc_valid(struct Alloc* alloc) {
    return heap_ptr_valid((char*)alloc);
}

inline bool heap_ptr_valid(char* ptr) {
    return ((heap.side == LEFT && ptr < heap.data + heap.size / 2)
        || (heap.side == RIGHT && ptr >= heap.data + heap.size / 2));
}

struct Alloc* heap_realloc(struct Alloc* alloc) {
    // alloc is invalid
    if (alloc == NULL || alloc->data == NULL) 
        return NULL;

    if (heap_alloc_valid(alloc))
        return alloc;

    // if the alloc's data is pointing into the new heap side it means that pointer has already been re-alloced
    if (heap_ptr_valid(alloc->data))
        return (Alloc*)alloc->data;

    Alloc* newAlloc = (Alloc*)heap.first_free;
    newAlloc->size = alloc->size;
    newAlloc->data = heap.first_free + sizeof(Alloc);
    memcpy(newAlloc->data, alloc->data, alloc->size);
    // point the old allocs data to the re-alloced location
    alloc->data = (char*)newAlloc;
    heap.first_free += alloc->size + sizeof(Alloc);
    _allocations++;
    return newAlloc;
}