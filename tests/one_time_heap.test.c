#include <stdbool.h>
#include "unity.h"
#include "one_time_heap.h"

void test_one_time_heap_init(void)
{
    uint8_t heap_buffer[12345];
    memset(heap_buffer, 0x23, sizeof(heap_buffer));
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    uint8_t zeroes[sizeof(heap_buffer)];
    memset(zeroes, 0, sizeof(zeroes));
    TEST_ASSERT_EQUAL_MEMORY(zeroes, heap_buffer, sizeof(heap_buffer));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_one_time_heap_init);

    UNITY_END();

    return 0;
}
