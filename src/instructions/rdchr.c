#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push_ref(vm, getchar());
}

void rdchr_init(const char* name) {
    DEFAULT_INST(rdchr);
    instructions[RDCHR] = rdchr;
}