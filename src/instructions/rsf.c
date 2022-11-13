#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    vm->sp = vm->fp - 1;
    vm->fp = __vm_pop_value(vm);
}

void rsf_init(const char* name) {
    DEFAULT_INST(rsf);
    instructions[RSF] = rsf;
}