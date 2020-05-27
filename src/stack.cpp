#include "lambda.h"

void stack_push(Stack *stack, void* element) {
    stack->top = (char *)stack->top + stack->elementsize;
    memcpy(stack->top, element, stack->elementsize);
}

void *stack_pop(Stack *stack) {
    stack->top = (char *)stack->top - stack->elementsize;
    return (void *)((char *)stack->top + stack->elementsize);
}

bool stack_empty(Stack *stack) {
   return (stack->top == (char *)stack->data - stack->elementsize);
}

Stack *stack_create_empty(size_t elements, size_t elementsize) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->elements = elements;
    stack->elementsize = elementsize;
    stack->data = malloc(elementsize * elements);
    stack->top = (char *)stack->data - elementsize;
    return stack;
}
