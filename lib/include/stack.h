#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdint.h>
typedef struct {
    uint8_t *top;
    size_t size;
    size_t capacity;
} uint8stack;
int uint8stack_create(uint8stack *s, uint8_t *top, size_t size, size_t capacity);
int uint8stack_destroy(uint8stack *s);
int uint8stack_push(uint8stack *s, uint8_t value);
int uint8stack_pop(uint8stack *s, uint8_t *value);
#endif
