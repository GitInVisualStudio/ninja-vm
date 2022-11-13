#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int pos = PARAMETER(op);
    vm->cp = pos;
}

void jmp_init(const char* name) {
    DEFAULT_INST(jmp);
    instructions[JMP] = jmp;
}