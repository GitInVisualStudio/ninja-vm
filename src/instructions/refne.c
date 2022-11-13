#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* ref1 = __vm_pop(vm);
    Object* ref2 = __vm_pop(vm);
    if (IS_PRIMITIVE(ref1) || IS_PRIMITIVE(ref2))
        fatalError("Can't compare ref and primitive");
    __vm_push_ref(vm, ref1->types.ref != ref2->types.ref);
}

void refne_init(const char* name) {
    DEFAULT_INST(refne);
    instructions[REFNE] = refne;
}