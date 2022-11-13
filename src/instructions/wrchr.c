#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    // this is < 256 anyways
    printf("%c", __vm_pop_value(vm));
}

void wrchr_init(const char* name) {
    DEFAULT_INST(wrchr);
    instructions[WRCHR] = wrchr;
}
