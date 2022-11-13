#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int n = PARAMETER(op);
    vm->stack[vm->fp + n] = *__vm_pop(vm);
}

void popl_init(const char* name) {
    DEFAULT_INST(popl);
    instructions[POPL] = popl;
}