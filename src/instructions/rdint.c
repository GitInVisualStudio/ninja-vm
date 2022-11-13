#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    bigRead(stdin);
    __vm_push_bip(vm);
}

void rdint_init(const char* name) {
    DEFAULT_INST(rdint);
    instructions[RDINT] = rdint;
}