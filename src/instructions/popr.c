#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    vm->reg = *__vm_pop(vm);
}

void popr_init(const char* name) {
    DEFAULT_INST(popr);
    instructions[POPR] = popr;
}