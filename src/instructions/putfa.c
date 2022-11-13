#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* value = __vm_pop(vm);
    int index = __vm_pop_value(vm);
    Object* array = __vm_pop(vm);
    if (IS_PRIMITIVE(array))
        fatalError("Tried to index primitive as array");
    int arrayLength = getPrimSize(array->types.ref) / sizeof(Object);
    if (index < 0 || index >= arrayLength)
        fatalError("Tried to access element outside of array");
    Object* elements = (Object*)getPrimObjectDataPointer(array->types.ref);
    elements[index] = *value;
}

void putfa_init(const char* name) {
    DEFAULT_INST(putfa);
    instructions[PUTFA] = putfa;
}