#include <assert.h>
int STACK[] = { 67, 69, 420, 1337 };
int STACK_MAX_LEN = sizeof(STACK)/sizeof(*STACK);

struct stack_int {
    int* ptr;
    int len;
};

// Pops the last element of the array
int stack_int_pop(struct stack_int* stack) {
    // Popping an item off a stack just involves removing the last item from the list
    // and decrementing the length counter
    return stack->ptr[--stack->len];
}

// Pushes an element to the array
void stack_int_push(struct stack_int* stack, int value) {
    // If our stack had a capacity we would
    stack->ptr[stack->len++] = value;
}

// We can build a Stack on top of an array by restricting arbitrary set and get operations to only work on the last element
int main(void) {
    struct stack_int stack = { .ptr = STACK, .len = STACK_MAX_LEN };
    int a = stack_int_pop(&stack);
    assert(a == 1337);
    
    stack_int_push(&stack, 70);
    a = stack_int_pop(&stack);
    assert(a == 70);

    return 0;
}