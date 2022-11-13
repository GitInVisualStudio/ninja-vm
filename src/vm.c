#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

// defined in vm.h
Instruction instructions[MAX_INSTRUCTIONS];


void vm_init(struct VM* vm, int stackSizeKiB) {
    instruction_init();
    printf("%s\n", "Ninja Virtual Machine started");
    const int bytes = (1024 * stackSizeKiB); 
    vm->sp = vm->fp = vm->cp = 0;
    vm->stack = (Object*)calloc(bytes, 1);
    vm->stack_size = bytes / sizeof(Object);
}

void vm_delete(struct VM* vm) {
    if (vm->instructions)
        free(vm->instructions);
    if (vm->variables)
        free(vm->variables);
    free(vm->stack);
    printf("%s\n", "Ninja Virtual Machine stopped");
}

int vm_run(struct VM* self) {
    while (self->cp < self->n_instructions) {
        unsigned int op = self->instructions[self->cp++];
        OPCODE opcode = GET_OPCODE(op);
        instructions[opcode].process(self, op);
        if (opcode == HALT) 
            return 0;
    }
    return 1;
}

int vm_load_binary(struct VM* self, char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: cannot open code file '%s'\n", path);
        return 1;
    }

    char buffer[4];
    size_t read = fread(buffer, sizeof(buffer), 1, fp); // IDENTIFIER
    if (read != 1) return -1;
    if (strncmp(buffer, "NJBF", 4) != 0) {
        printf("Error: file '%s' is not a Ninja binary\n", path);
        return 2;
    }

    int version;
    read = fread(&version, sizeof(int), 1, fp);
    if (version > VERSION) {
        printf("Not the same VERSION (current version: %i, got: %i)!\n", VERSION, version);
        return 3;
    }

    read = fread(&self->n_instructions, sizeof(self->n_instructions), 1, fp);

    read = fread(&self->n_variables, sizeof(self->n_variables), 1, fp);
    // set every variable to NULL
    self->variables = (struct Object*)calloc(sizeof(Object), self->n_variables);

    self->instructions = (unsigned int*)malloc(sizeof(unsigned int) * self->n_instructions);
    read = fread(self->instructions, sizeof(unsigned int), self->n_instructions, fp);

    fclose(fp);
    return 0;
}

inline Object* __vm_pop(struct VM* self) {
    return &self->stack[self->sp--];
}

inline int __vm_pop_value(struct VM* self) {
    Object* obj = &self->stack[self->sp--];
    if (obj->type == INT)
        return obj->types.value;
    if (!IS_PRIMITIVE(obj))
        fatalError("Tried to access obj as primitive");
    bip.op1 = obj->types.ref;
    return bigToInt();
}

inline void __vm_push(struct VM* self, Object value) {
    if (self->sp >= self->stack_size)
        fatalError("Stack overflow");
    self->stack[++self->sp] = value;
}

inline void __vm_push_value(struct VM* self, int value) {
    __vm_push(self, (Object){.type = INT, .types.value = value});
}

inline void __vm_push_ref(struct VM* self, int value) {
    bigFromInt(value);
    __vm_push_bip(self);
}

inline void __vm_push_bip(struct VM* self) {
    // push the result of bip as a primitive
    __vm_push(self, (Object){.type = PRIM, .types.ref = bip.res});
}

inline int __big_cmp(ObjRef ref) {
    #define BIG_PTR(bigObjRef)			((Big *) (getPrimObjectDataPointer(bigObjRef)))
    #define GET_ND(bigObjRef)			(BIG_PTR(bigObjRef)->nd)
    #define GET_SIGN(bigObjRef)		    (BIG_PTR(bigObjRef)->sign)
    /**
     * NOTE: if you look at the BigInt format you can see that the number 0 will be stored with nd = 0 (see bigint.c:803 void bigFromInt(int n)...)
     * meaning we can check if the number is positive or negative by just multiplying the ints number of digits (nd) with its sign
     */
    return GET_ND(ref) * (GET_SIGN(ref) * 2 - 1);
    #undef BIG_PTR
    #undef GET_ND
    #undef GET_SIGN
}

inline void __vm_bip_op(struct VM* self, void(*operation)(void)) {
    Object* b = __vm_pop(self);
    Object* a = __vm_pop(self);
    if (!IS_PRIMITIVE(a) || !IS_PRIMITIVE(b))
        fatalError("Can only operate on primitves!");
    bip.op1 = GET_PRIMITIVE(a);
    bip.op2 = GET_PRIMITIVE(b);
    operation();
}

inline int __vm_bip_op_i(struct VM* self, int(*operation)(void)) {
    Object* b = __vm_pop(self);
    Object* a = __vm_pop(self);
    if (!IS_PRIMITIVE(a) || !IS_PRIMITIVE(b))
        fatalError("Can only operate on primitves!");
    bip.op1 = GET_PRIMITIVE(a);
    bip.op2 = GET_PRIMITIVE(b);
    return operation();
}

void vm_realloc(struct VM* vm) {
    bip.op1 = heap_realloc(bip.op1);
    bip.op2 = heap_realloc(bip.op2);
    bip.res = heap_realloc(bip.res);
    bip.rem = heap_realloc(bip.rem);

    if(vm->reg.type != INT)
        __vm_realloc_obj(&vm->reg);

    for(int i = 0; i <= vm->sp; i++) 
        __vm_realloc_obj(&vm->stack[i]);
        
    for(int i = 0; i < vm->n_variables; i++) 
        __vm_realloc_obj(&vm->variables[i]);
}

void __vm_realloc_obj(Object* obj) {
    if (obj == NULL)
        return;
    if (obj->type == INT)
        return;
    if (obj->types.ref == NULL)
        return;
    if (heap_alloc_valid(obj->types.ref))
        return;
    if (obj->type == REF) {
        const int size = getPrimSize(obj->types.ref) / sizeof(Object);
        Object* objs = (Object*)getPrimObjectDataPointer(obj->types.ref);
        for (int k = 0; k < size; k++)
            __vm_realloc_obj(&objs[k]);
    }

    // NOTE: we have to set the objs ptr after we process all its fields/elements
    obj->types.ref = heap_realloc(obj->types.ref);
}