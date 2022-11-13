#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int n = PARAMETER(op);
    vm->sp -= n;
}

void drop_init(const char* name) {
    DEFAULT_INST(drop);
    instructions[DROP] = drop;
}