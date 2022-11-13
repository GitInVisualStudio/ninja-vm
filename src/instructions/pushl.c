#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int n = PARAMETER(op);
    __vm_push(vm, vm->stack[vm->fp + n]);
}

void pushl_init(const char* name) {
    DEFAULT_INST(pushl);
    instructions[PUSHL] = pushl;
}