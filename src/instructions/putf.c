#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* value = __vm_pop(vm);
    Object* obj = __vm_pop(vm);
    if (IS_PRIMITIVE(obj))
        fatalError("Tried to put field of primitive\n");
    int n = PARAMETER(op);
    int fieldLength = getPrimSize(obj->types.ref) / sizeof(Object);
    if (n < 0 || n >= fieldLength)
        fatalError("Tried to access field outside of object");
    Object* fields = (Object*)getPrimObjectDataPointer(obj->types.ref);

    fields[n] = *value;
}

void putf_init(const char* name) {
    DEFAULT_INST(putf);
    instructions[PUTF] = putf;
}