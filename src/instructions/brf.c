#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    int pos = PARAMETER(op);
    ObjRef ref = GET_PRIMITIVE(__vm_pop(vm));
    if (ref == NULL) 
        fatalError("null ptr ref!");
    if (!__big_cmp(ref))
        vm->cp = pos;
}

void brf_init(const char* name) {
    DEFAULT_INST(brf);
    instructions[BRF] = brf;
}