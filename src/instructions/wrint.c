#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* obj = __vm_pop(vm);
    if (!IS_PRIMITIVE(obj))
        fatalError("Can't operate on Object");
    bip.op1 = GET_PRIMITIVE(obj);
    bigPrint(stdout);
}

void wrint_init(const char* name) {
    DEFAULT_INST(wrint);
    instructions[WRINT] = wrint;
}