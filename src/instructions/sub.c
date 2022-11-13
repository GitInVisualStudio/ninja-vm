#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_bip_op(vm, bigSub);
    __vm_push_bip(vm);
}

void sub_init(const char* name) {
    DEFAULT_INST(sub);
    instructions[SUB] = sub;
}