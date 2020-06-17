#include "lambda.h"

void Li::stack_push(Lt::Stack *stack, void* element) {
    stack->top = (char *)stack->top + stack->elementsize;
    memcpy(stack->top, element, stack->elementsize);
}

void *Li::stack_pop(Lt::Stack *stack) {
    stack->top = (char *)stack->top - stack->elementsize;
    return (void *)((char *)stack->top + stack->elementsize);
}

bool Li::stack_empty(Lt::Stack *stack) {
   return (stack->top == (char *)stack->data - stack->elementsize);
}

Lt::Stack *Li::stack_create_empty(size_t elements, size_t elementsize) {
    Lt::Stack *stack = (Lt::Stack *)malloc(sizeof(Lt::Stack));
    stack->elements = elements;
    stack->elementsize = elementsize;
    stack->data = malloc(elementsize * elements);
    stack->top = (char *)stack->data - elementsize;
    return stack;
}
