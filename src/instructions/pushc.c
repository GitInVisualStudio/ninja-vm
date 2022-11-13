#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push_ref(vm, PARAMETER(op));
}

void pushc_init(const char* name) {
    DEFAULT_INST(pushc);
    instructions[PUSHC] = pushc;
}