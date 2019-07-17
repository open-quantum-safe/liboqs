/**
 *  stack.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __NTSKEM_STACK_H
#define __NTSKEM_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct node {
    void *data;
    struct node *prev;
} node;

typedef struct stack {
    node *top;
    size_t size;
} stack;

stack* PQCLEAN_NTSKEM1264_CLEAN_stack_create(void);

void PQCLEAN_NTSKEM1264_CLEAN_stack_free(stack *stack);

int PQCLEAN_NTSKEM1264_CLEAN_stack_push(stack *stack, void *ptr);

void* PQCLEAN_NTSKEM1264_CLEAN_stack_pop(stack *stack);

size_t PQCLEAN_NTSKEM1264_CLEAN_stack_size(const stack *stack);

#endif /* __NTSKEM_STACK_H */
