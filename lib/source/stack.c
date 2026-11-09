#include "../include/stack.h"
#define OVERFlOW -1
#define UNDERFLOW -2
int uint8stack_create(uint8stack *s, uint8_t *top, size_t size, size_t capacity) {
    s->top = top;
    s->size = size;
    s->capacity = capacity;
    return 0; 
}

int uint8stack_destroy(uint8stack *s) {
    s->top = NULL;
    s->size = 0;
    s->capacity = 0;
    return 0;
}
int uint8stack_push(uint8stack *s, uint8_t value) {
    if (s->size >= s->capacity) {
        return OVERFlOW; // Stack overflow
    }
    s->top[s->size++] = value;
    return 0;   
}
int uint8stack_pop(uint8stack *s, uint8_t *value) {
    if (s->size == 0) {
        return UNDERFLOW; // Stack underflow
    }
    *value = s->top[--s->size];
    return 0;
}
