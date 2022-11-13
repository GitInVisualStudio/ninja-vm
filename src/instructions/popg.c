#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int index = PARAMETER(op);
    if (index < 0 || index >= vm->n_variables)
        fatalError("Tried to access variable outside of scope!");
    vm->variables[index] = *__vm_pop(vm);
}

void popg_init(const char* name) {
    DEFAULT_INST(popg);
    instructions[POPG] = popg;
}