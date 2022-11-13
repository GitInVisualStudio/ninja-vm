#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Alloc {
    unsigned int size;
    char* data;
} Alloc;

/**
 * has to realloc all the ptrs that will be kept after the collection
 */
typedef void (*realloc_callback)(void);

typedef struct Heap {
    char* data;
    char* first_free;
    unsigned int size;
    char side;
    struct AllocsList {
        unsigned int length, index;
        // every alloc has to be a pointer itselfe so we can change to pointed resource location for everyone
        Alloc** allocs;
    } list;

    /**
     * Will be invoked every time the heap tries to collect garbage.
     * This way the heap is completely independet from its environment and
     * it's the environments job to tell which resources are still used.
     * 
     * This is useful if we f.e. want to use one heap for multiple VMs.
     */
    realloc_callback set_used;
    bool debug, purge;
} Heap;

extern Heap heap;

void heap_init(realloc_callback is_used, int sizeKiB);
void heap_delete();
void* heap_alloc(unsigned int size);
/**
 * @brief reallocs the alloc pointer and its data after garbage collection
 * 
 * @param alloc the old Allocs ptr
 * @return struct Alloc* the new Alocs ptr
 */
struct Alloc* heap_realloc(struct Alloc* alloc);

/**
 * intern functions
 */
void __heap_append(struct Alloc* alloc);
void __heap_collect();
bool heap_alloc_valid(struct Alloc* alloc);
bool heap_ptr_valid(char* alloc);
extern bool __heap_full(unsigned int size);

#endif