#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push(vm, (Object){.type = REF, .types.ref = NULL});
}

void pushn_init(const char* name) {
    DEFAULT_INST(pushn);
    instructions[PUSHN] = pushn;
}