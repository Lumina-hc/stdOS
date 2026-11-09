#include "../include/stack.h"
#include <stdint.h>
#include <stdio.h>

static int tests_run;
static int tests_failed;

#define CHECK(cond, msg)                                          \
    do {                                                          \
        tests_run++;                                              \
        if (!(cond)) {                                            \
            tests_failed++;                                       \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);       \
        }                                                         \
    } while (0)

static void test_create(void) {
    uint8stack s;
    uint8_t buf[4] = {9, 8, 7, 6};
    CHECK(uint8stack_create(&s, buf, 0, 4) == 0, "create ok");
    CHECK(s.size == 0, "size starts at 0");
    CHECK(s.capacity == 4, "capacity is 4");
    CHECK(s.top == buf, "top points to buffer");
}

static void test_create_with_initial_size(void) {
    uint8stack s;
    uint8_t buf[4] = {1, 2, 3, 4};
    CHECK(uint8stack_create(&s, buf, 2, 4) == 0, "create ok");
    CHECK(s.size == 2, "size is 2");
    uint8_t v;
    CHECK(uint8stack_pop(&s, &v) == 0, "pop ok");
    CHECK(v == 2, "first popped is buf[1] = 2");
    CHECK(uint8stack_pop(&s, &v) == 0, "pop ok");
    CHECK(v == 1, "next popped is buf[0] = 1");
}

static void test_push_pop_lifo(void) {
    uint8stack s;
    uint8_t buf[4];
    CHECK(uint8stack_create(&s, buf, 0, 4) == 0, "create ok");
    CHECK(uint8stack_push(&s, 10) == 0, "push 10 ok");
    CHECK(uint8stack_push(&s, 20) == 0, "push 20 ok");
    CHECK(uint8stack_push(&s, 30) == 0, "push 30 ok");
    CHECK(s.size == 3, "size is 3 after 3 pushes");
    uint8_t v;
    CHECK(uint8stack_pop(&s, &v) == 0, "pop ok");
    CHECK(v == 30, "LIFO: last in is 30");
    CHECK(uint8stack_pop(&s, &v) == 0, "pop ok");
    CHECK(v == 20, "LIFO: next is 20");
    CHECK(uint8stack_pop(&s, &v) == 0, "pop ok");
    CHECK(v == 10, "LIFO: first in is 10");
    CHECK(s.size == 0, "size back to 0");
}

static void test_push_full(void) {
    uint8stack s;
    uint8_t buf[3];
    CHECK(uint8stack_create(&s, buf, 0, 3) == 0, "create ok");
    CHECK(uint8stack_push(&s, 1) == 0, "push ok");
    CHECK(uint8stack_push(&s, 2) == 0, "push ok");
    CHECK(uint8stack_push(&s, 3) == 0, "push ok");
    CHECK(uint8stack_push(&s, 4) == -1, "push on full stack returns OVERFLOW (-1)");
    uint8_t v;
    CHECK(uint8stack_pop(&s, &v) == 0, "pop after full ok");
    CHECK(v == 3, "LIFO order preserved when full");
    CHECK(uint8stack_push(&s, 4) == 0, "push after pop ok");
}

static void test_pop_empty(void) {
    uint8stack s;
    uint8_t buf[2];
    CHECK(uint8stack_create(&s, buf, 0, 2) == 0, "create ok");
    uint8_t v = 0xFF;
    CHECK(uint8stack_pop(&s, &v) == -2, "pop from empty returns UNDERFLOW (-2)");
    CHECK(v == 0xFF, "value unchanged on underflow");
}

static void test_destroy_resets(void) {
    uint8stack s;
    uint8_t buf[4];
    CHECK(uint8stack_create(&s, buf, 0, 4) == 0, "create ok");
    CHECK(uint8stack_destroy(&s) == 0, "destroy ok");
    CHECK(s.size == 0, "size reset to 0");
    CHECK(s.capacity == 0, "capacity reset to 0");
    CHECK(s.top == NULL, "top reset to NULL");
}

int main(void) {
    test_create();
    test_create_with_initial_size();
    test_push_pop_lifo();
    test_push_full();
    test_pop_empty();
    test_destroy_resets();

    printf("%d/%d passed, %d failed\n",
           tests_run - tests_failed, tests_run, tests_failed);
    return tests_failed ? 1 : 0;
}
