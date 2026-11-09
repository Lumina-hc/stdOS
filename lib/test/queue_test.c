#include "../include/queue.h"
#include <stdint.h>
#include <stdio.h>

static int tests_run;
static int tests_failed;

#define CHECK(cond, msg)                                          \
    do {                                                          \
        tests_run++;                                              \
        if (!(cond)) {                                            \
            tests_failed++;                                        \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);       \
        }                                                         \
    } while (0)

static void test_destroy_resets(void) {
    uint8queue q;
    uint8_t buf[4] = {1, 2, 3, 4};
    CHECK(uint8queue_create(&q, buf, 0, 4) == 0, "create ok");
    CHECK(q.size == 0, "size starts at 0");
    CHECK(q.capacity == 4, "capacity is 4");
    CHECK(uint8queue_destroy(&q) == 0, "destroy ok");
    CHECK(q.size == 0, "size reset to 0");
    CHECK(q.capacity == 0, "capacity reset to 0");
    CHECK(q.top == NULL, "top reset to NULL");
}

static void test_push_pop_fifo(void) {
    uint8queue q;
    uint8_t buf[4];
    CHECK(uint8queue_create(&q, buf, 0, 4) == 0, "create ok");
    CHECK(uint8queue_push(&q, 10) == 0, "push 10 ok");
    CHECK(uint8queue_push(&q, 20) == 0, "push 20 ok");
    CHECK(uint8queue_push(&q, 30) == 0, "push 30 ok");
    uint8_t v;
    CHECK(uint8queue_pop(&q, &v) == 0, "pop ok");
    CHECK(v == 10, "FIFO: first out is 10");
    CHECK(uint8queue_pop(&q, &v) == 0, "pop ok");
    CHECK(v == 20, "FIFO: next is 20");
    CHECK(uint8queue_pop(&q, &v) == 0, "pop ok");
    CHECK(v == 30, "FIFO: last is 30");
}

static void test_push_full(void) {
    uint8queue q;
    uint8_t buf[3];
    CHECK(uint8queue_create(&q, buf, 0, 3) == 0, "create ok");
    CHECK(uint8queue_push(&q, 1) == 0, "push ok");
    CHECK(uint8queue_push(&q, 2) == 0, "push ok");
    CHECK(uint8queue_push(&q, 3) == 0, "push ok");
    uint8_t v;
    CHECK(uint8queue_push(&q, 4) == -1, "push on full queue fails");
    CHECK(uint8queue_pop(&q, &v) == 0, "pop after full ok");
    CHECK(v == 1, "FIFO order preserved when full");
    CHECK(uint8queue_push(&q, 4) == 0, "push after pop ok");
}

static void test_pop_empty(void) {
    uint8queue q;
    uint8_t buf[2];
    CHECK(uint8queue_create(&q, buf, 0, 2) == 0, "create ok");
    uint8_t v = 0xFF;
    CHECK(uint8queue_pop(&q, &v) == -1, "pop from empty fails");
}

static void test_create_validation(void) {
    uint8queue q;
    uint8_t buf[2];
    CHECK(uint8queue_create(NULL, buf, 0, 2) == -1, "create with NULL q fails");
    CHECK(uint8queue_create(&q, NULL, 0, 2) == -1, "create with NULL buf fails");
    CHECK(uint8queue_create(&q, buf, 0, 0) == -1, "create with capacity 0 fails");
}

int main(void) {
    test_destroy_resets();
    test_push_pop_fifo();
    test_push_full();
    test_pop_empty();
    test_create_validation();

    printf("%d/%d passed, %d failed\n",
           tests_run - tests_failed, tests_run, tests_failed);
    return tests_failed ? 1 : 0;
}
