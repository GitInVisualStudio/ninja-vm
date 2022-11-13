#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int fields = PARAMETER(op);
    if (fields <= 0)
        fatalError("Tried to make new object with <0 fields\n");
    Object obj = {
        .type = REF,
        .types.ref = newPrimObject(sizeof(Object) * fields)
    };
    __vm_push(vm, obj);
}

void new_init(const char* name) {
    DEFAULT_INST(new);
    instructions[NEW] = new;
}