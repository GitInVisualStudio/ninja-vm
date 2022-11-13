#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    __vm_push_value(vm, vm->fp);
    int n = PARAMETER(op);
    vm->fp = vm->sp + 1;
    vm->sp = vm->sp + n;
}

void asf_init(const char* name) {
    DEFAULT_INST(asf);
    instructions[ASF] = asf;
}