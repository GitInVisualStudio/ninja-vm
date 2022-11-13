#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_bip_op(vm, bigDiv);
    __vm_push_bip(vm);
}

void div_init(const char* name) {
    DEFAULT_INST(div);
    instructions[DIV] = div;
}