#include "../vm.h"

static void process(struct VM* vm, unsigned int op) {
    Object* ref = __vm_pop(vm);
    if (IS_PRIMITIVE(ref)) {
        __vm_push_ref(vm, -1);    
        return;
    }
    __vm_push_ref(vm, getPrimSize(ref->types.ref) / sizeof(Object));
}

void getsz_init(const char* name) {
    DEFAULT_INST(getsz);
    instructions[GETSZ] = getsz;
}