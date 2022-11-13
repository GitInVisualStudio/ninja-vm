#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_bip_op(vm, bigMul);
    __vm_push_bip(vm);
}

void mul_init(const char* name) {
    DEFAULT_INST(mul);
    instructions[MUL] = mul;
}