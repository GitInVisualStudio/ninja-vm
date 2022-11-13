#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int index = __vm_pop_value(vm);
    Object* array = __vm_pop(vm);
    if (IS_PRIMITIVE(array))
        fatalError("Tried to index primitive as array");
    int arrayLength = getPrimSize(array->types.ref) / sizeof(Object);
    if (index < 0 || index >= arrayLength)
        fatalError("Tried to access element outside of array");
    Object* elements = (Object*)getPrimObjectDataPointer(array->types.ref);
    __vm_push(vm, elements[index]);
}

void getfa_init(const char* name) {
    DEFAULT_INST(getfa);
    instructions[GETFA] = getfa;
}