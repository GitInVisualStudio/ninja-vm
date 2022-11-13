#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
}

void halt_init(const char* name) {
    DEFAULT_INST(halt);
    instructions[HALT] = halt;
}