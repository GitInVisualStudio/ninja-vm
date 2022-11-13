#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_bip_op(vm, bigDiv);
    __vm_push(vm, (Object){.type = PRIM, .types.ref = bip.rem});
}

void mod_init(const char* name) {
    DEFAULT_INST(mod);
    instructions[MOD] = mod;
}