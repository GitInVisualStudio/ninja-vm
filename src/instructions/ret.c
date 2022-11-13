#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    vm->cp = __vm_pop_value(vm);
}

void ret_init(const char* name) {
    DEFAULT_INST(ret);
    instructions[RET] = ret;
}