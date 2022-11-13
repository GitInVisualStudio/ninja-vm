#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* obj = __vm_pop(vm);
    if (IS_PRIMITIVE(obj))
        fatalError("Tried to get field of primitive\n");
    int n = PARAMETER(op);
    int fieldLength = getPrimSize(obj->types.ref) / sizeof(Object);;
    if (n < 0 || n >= fieldLength)
        fatalError("Tried to access field outside of object");
    Object* fields = (Object*)getPrimObjectDataPointer(obj->types.ref);
    __vm_push(vm, fields[n]);
}

void getf_init(const char* name) {
    DEFAULT_INST(getf);
    instructions[GETF] = getf;
}