#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push(vm, vm->stack[vm->sp]);
}

void dup_init(const char* name) {
    DEFAULT_INST(dup);
    instructions[DUP] = dup;
}