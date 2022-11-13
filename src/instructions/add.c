#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_bip_op(vm, bigAdd);
    __vm_push_bip(vm);
}

void add_init(const char* name) {
    DEFAULT_INST(add);
    instructions[ADD] = add;
}