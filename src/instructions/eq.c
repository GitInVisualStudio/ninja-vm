#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push_ref(vm, __vm_bip_op_i(vm, bigCmp) == 0);
}

void eq_init(const char* name) {
    DEFAULT_INST(eq);
    instructions[EQ] = eq;
}