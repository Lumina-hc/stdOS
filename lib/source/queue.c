#include "../include/queue.h"

int uint8queue_create(uint8queue *q, uint8_t *top, size_t size, size_t capacity) {
    if (!q || !top || capacity == 0) return -1;
    q->top = top;
    q->head = 0;
    q->size = size;
    q->capacity = capacity;
    return 0;
}

int uint8queue_destroy(uint8queue *q) {
    q->top = NULL;
    q->head = 0;
    q->size = 0;
    q->capacity = 0;
    return 0;
}

int uint8queue_push(uint8queue *q, uint8_t value) {
    if (q->size == q->capacity) return -1;
    size_t next = (q->head + q->size) % q->capacity;
    q->top[next] = value;
    q->size++;
    return 0;
}

int uint8queue_pop(uint8queue *q, uint8_t *value) {
    if (q->size == 0) return -1;
    *value = q->top[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return 0;
}
