#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int index = PARAMETER(op);
    if (index < 0 || index >= vm->n_variables)
        fatalError("Tried to access variable outside of scope!");
    __vm_push(vm, vm->variables[index]);
}

void pushg_init(const char* name) {
    DEFAULT_INST(pushg);
    instructions[PUSHG] = pushg;
}