#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils/bigint.h"

#define VERSION 8

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))
#define PARAMETER(x) SIGN_EXTEND(IMMEDIATE(x))

typedef enum OPCODE {
    HALT = 0,
    PUSHC,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    RDINT,
    WRINT,
    RDCHR,
    WRCHR,
    PUSHG,
    POPG,
    ASF,
    RSF,
    PUSHL,
    POPL,
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,
    JMP,
    BRF,
    BRT,
    CALL,
    RET,
    DROP,
    PUSHR,
    POPR,
    DUP,
    NEW,
    GETF,
    PUTF,
    NEWA,
    GETFA,
    PUTFA,
    GETSZ,
    PUSHN,
    REFEQ,
    REFNE,
    OPCODE_END
} OPCODE;

#define IS_PRIMITIVE(x) (x->type == PRIM)
#define GET_PRIMITIVE(x) (x->types.ref)
#define GET_OPCODE(op) (op >> 24 & 0xFF)

typedef enum TYPE {
    INT = 0,
    REF,
    PRIM
} TYPE;

typedef struct Object {
    TYPE type;
    union {
        int value;
        ObjRef ref;
    } types;
} Object;

typedef struct VM {
    int sp;
    int fp;
    int cp;
    Object* stack;
    int stack_size;
    Object* variables;
    unsigned int* instructions;
    unsigned int n_instructions, n_variables;
    Object reg;
} VM;

typedef void(*process_op_ptr)(struct VM* vm, unsigned int op);

typedef struct Instruction {
    char name[32];
    process_op_ptr process;
} Instruction;

#define DEFAULT_INST(_var)\
    struct Instruction _var = {.process = process};\
    strcpy(_var.name, name);\

#define MAX_INSTRUCTIONS OPCODE_END
extern Instruction instructions[MAX_INSTRUCTIONS];

#define _INSTRUCTION_DECL(name) \
    extern void name##_init(const char*);\
    name##_init(#name);

static inline void instruction_init() {
    _INSTRUCTION_DECL(halt);
    _INSTRUCTION_DECL(pushc);
    _INSTRUCTION_DECL(add);
    _INSTRUCTION_DECL(sub);
    _INSTRUCTION_DECL(mul);
    _INSTRUCTION_DECL(div);
    _INSTRUCTION_DECL(mod);
    _INSTRUCTION_DECL(rdint);
    _INSTRUCTION_DECL(wrint);
    _INSTRUCTION_DECL(rdchr);
    _INSTRUCTION_DECL(wrchr);
    _INSTRUCTION_DECL(pushg);
    _INSTRUCTION_DECL(popg);
    _INSTRUCTION_DECL(asf);
    _INSTRUCTION_DECL(rsf);
    _INSTRUCTION_DECL(pushl);
    _INSTRUCTION_DECL(popl);
    _INSTRUCTION_DECL(eq);
    _INSTRUCTION_DECL(ne);
    _INSTRUCTION_DECL(lt);
    _INSTRUCTION_DECL(le);
    _INSTRUCTION_DECL(gt);
    _INSTRUCTION_DECL(ge);
    _INSTRUCTION_DECL(jmp);
    _INSTRUCTION_DECL(brf);
    _INSTRUCTION_DECL(brt);
    _INSTRUCTION_DECL(call);
    _INSTRUCTION_DECL(ret);
    _INSTRUCTION_DECL(drop);
    _INSTRUCTION_DECL(pushr);
    _INSTRUCTION_DECL(popr);
    _INSTRUCTION_DECL(dup);
    _INSTRUCTION_DECL(new);
    _INSTRUCTION_DECL(getf);
    _INSTRUCTION_DECL(putf);
    _INSTRUCTION_DECL(newa);
    _INSTRUCTION_DECL(getfa);
    _INSTRUCTION_DECL(putfa);
    _INSTRUCTION_DECL(getsz);
    _INSTRUCTION_DECL(pushn);
    _INSTRUCTION_DECL(refeq);
    _INSTRUCTION_DECL(refne);
}

/**
 * public to use vm methods
 */

void vm_init(struct VM* vm, int stackSizeKiB);
void vm_delete(struct VM* vm);
/**
 * @return int if exit normally or not
 */
int vm_run(struct VM* self);
/**
 * @return int if binary was loaded sucessfully
 */
int vm_load_binary(struct VM* vm, char* path);
void vm_realloc(struct VM* vm);

/**
 * intern vm methods
 */

Object* __vm_pop(struct VM* self);
int __vm_pop_value(struct VM* self);
void __vm_push(struct VM* self, Object obj);
void __vm_push_value(struct VM* self, int value);
void __vm_push_ref(struct VM* self, int value);
void __vm_push_bip(struct VM* self);
void __vm_bip_op(struct VM* self, void(*operation)(void));
int __vm_bip_op_i(struct VM* self, int(*operation)(void));
void __vm_realloc_obj(Object* obj);

/**
 * @brief compares the BigInt against 0.
 * 
 * this can be implemented by allocating another BigInt with all 0s and use the built in functions,
 * but that just seems so unreasonably stupid that i don't want to do it.
 * 
 * @return int <=0 false, >0 true
 */
int __big_cmp(ObjRef obj);

#endif