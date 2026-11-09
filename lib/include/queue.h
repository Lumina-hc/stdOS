#ifndef QUEUE_H
#define QUEUE_H
#include <stdint.h>
#include <stddef.h>
typedef struct {
    uint8_t *top;
    size_t head;
    size_t size;
    size_t capacity;    
} uint8queue;
int uint8queue_create(uint8queue *q, uint8_t *top, size_t size, size_t capacity);
int uint8queue_destroy(uint8queue *q);
int uint8queue_push(uint8queue *q, uint8_t value);
int uint8queue_pop(uint8queue *q, uint8_t *value);

#endif