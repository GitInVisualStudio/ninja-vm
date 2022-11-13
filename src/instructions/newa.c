#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int elements = __vm_pop_value(vm);
    if (elements < 0)
        fatalError("Tried to make new array with <0 elements\n");
    Object obj = {
        .type = REF,
        .types.ref = newPrimObject(sizeof(Object) * elements)
    };
    __vm_push(vm, obj);
}

void newa_init(const char* name) {
    DEFAULT_INST(newa);
    instructions[NEWA] = newa;
}