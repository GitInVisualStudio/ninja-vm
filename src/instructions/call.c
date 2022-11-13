#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push_value(vm, vm->cp);
    vm->cp = PARAMETER(op);
}

void call_init(const char* name) {
    DEFAULT_INST(call);
    instructions[CALL] = call;
}