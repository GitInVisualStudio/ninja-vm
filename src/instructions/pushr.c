#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push(vm, vm->reg);
}

void pushr_init(const char* name) {
    DEFAULT_INST(pushr);
    instructions[PUSHR] = pushr;
}