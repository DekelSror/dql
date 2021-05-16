#include <stdlib.h>

#define stack_struct(type)\
typedef struct\
{\
    size_t _top;\
    size_t _capacity;\
    type _stack[];\
} type##_stack;

#define stack_create(type)\
static type##_stack* type##_create(size_t capacity)\
{\
    type##_stack* stack = (type##_stack*)malloc(sizeof(*stack) + capacity * sizeof(type));\
    stack->_capacity = capacity;\
    stack->_top = 0;\
    return stack;\
}

#define stack_free(type) static void type##_free(type##_stack* stack) {free(stack);}

#define stack_push(type)\
static int type##_push(type##_stack* stack, type elem)\
{\
    if (stack->_top >= stack->_capacity) return 1;\
    stack->_stack[stack->_top++] = elem;\
    return 0;\
}

#define stack_pop(type)\
static type type##_pop(type##_stack* stack)\
{\
    if (type##_is_empty(stack)) return NULL;\
    return stack->_stack[--stack->_top];\
}

#define stack_size(type) static size_t type##_size(const type##_stack* stack) { return stack->_top; }

#define stack_is_empty(type) static int type##_is_empty(const type##_stack* stack) { return stack->_top == 0; }

#define stack_classobj(type)\
typedef struct\
{\
    type##_stack*(*create)(size_t capacity);\
    void(*free)(type##_stack*);\
    int(*push)(type##_stack*, type elem);\
    type(*pop)(type##_stack*);\
    size_t(*size)(const type##_stack*);\
    int(*is_empty)(const type##_stack*);\
} type##_stack_api;\
const type##_stack_api type##Stack = {\
    type##_create,\
    type##_free,\
    type##_push,\
    type##_pop,\
    type##_size,\
    type##_is_empty\
};

#define make_stack(type)\
stack_struct(type)  \
stack_create(type)  \
stack_free(type)  \
stack_push(type)  \
stack_is_empty(type)  \
stack_pop(type)  \
stack_size(type)  \
stack_classobj(type)




