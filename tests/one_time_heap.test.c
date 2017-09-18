#include <stdbool.h>
#include <string.h>

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

void test_one_time_heap_count_available__matches_init(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(sizeof(heap_buffer),
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
}

void test_one_time_heap_count_available__matches_init_empty(void)
{
    uint8_t heap_buffer[1];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, 0);

    TEST_ASSERT_EQUAL(0,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
}

void test_one_time_heap_disable(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(sizeof(heap_buffer),
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    one_time_heap_disable(&heap);

    TEST_ASSERT_EQUAL(0,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
}

void test_one_time_heap_alloc(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(12345,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    void *buffer = one_time_heap_alloc(&heap, 279);
    memset(buffer, 0x11, 279);
   
    size_t count = 0; 
    for(size_t i=0;i<12345;i++) {
        TEST_ASSERT((heap_buffer[i] == 0) || (heap_buffer[i] == 0x11));
        if(heap_buffer[i] != 0) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL(279, count);
    
    size_t max_available = (12345-279);
    size_t min_available = max_available - 7;
    TEST_ASSERT(max_available >= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT(min_available <= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
}

void test_one_time_heap_alloc__zero__fails(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(12345, one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT_EQUAL(NULL,  one_time_heap_alloc(&heap, 0));
    TEST_ASSERT_EQUAL(12345, one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
}

void test_one_time_heap_alloc__too_much__fails(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(12345,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    one_time_heap_alloc(&heap, 279);
  
    size_t max_available = (12345-279);
    size_t min_available = max_available - 7;
    TEST_ASSERT(max_available >= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT(min_available <= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    void *fail = one_time_heap_alloc(&heap,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT) + 1);
    TEST_ASSERT_EQUAL(NULL, fail);
}

void test_one_time_heap_alloc__disabled__fails(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(12345,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    one_time_heap_alloc(&heap, 279);
  
    size_t max_available = (12345-279);
    size_t min_available = max_available - 7;
    TEST_ASSERT(max_available >= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT(min_available <= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    one_time_heap_disable(&heap);

   TEST_ASSERT_EQUAL(NULL, one_time_heap_alloc(&heap,
               one_time_heap_count_available(&heap,
                   ONE_TIME_HEAP_DEFAULT_ALIGNMENT)));
   TEST_ASSERT_EQUAL(NULL, one_time_heap_alloc(&heap, 1));
}

void test_one_time_heap_alloc__just_enough__ok(void)
{
    uint8_t heap_buffer[12345];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(12345,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    one_time_heap_alloc(&heap, 279);
  
    size_t max_available = (12345-279);
    size_t min_available = max_available - 7;
    TEST_ASSERT(max_available >= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT(min_available <= one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    void *ok = one_time_heap_alloc(&heap,
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT_NOT_NULL(ok);

    TEST_ASSERT_EQUAL(0, one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));
    TEST_ASSERT_EQUAL(NULL, one_time_heap_alloc(&heap, 1));
}

void test_one_time_heap_alloc__aligned_to_8_bytes(void)
{
    uint8_t heap_buffer[((279+7)*17)];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(((279+7)*17),
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    for(int i=0;i<17;i++) {
        void* allocated = one_time_heap_alloc(&heap, 275);
        TEST_ASSERT_NOT_NULL(allocated);
        TEST_ASSERT_EQUAL(0, (((uintptr_t)allocated) % 8));
    }
}

void test_one_time_heap_alloc_aligned__aligned_to_n_bytes(void)
{
    uint8_t heap_buffer[((279+1023)*17)];
    OneTimeHeap heap;

    one_time_heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    TEST_ASSERT_EQUAL(((279+1023)*17),
            one_time_heap_count_available(&heap,
                ONE_TIME_HEAP_DEFAULT_ALIGNMENT));

    for(int i=0;i<17;i++) {
        void* allocated = one_time_heap_alloc_aligned(&heap, 275, 1024);
        TEST_ASSERT_NOT_NULL(allocated);
        TEST_ASSERT_EQUAL(0, (((uintptr_t)allocated) % 1024));
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_one_time_heap_init);

    RUN_TEST(test_one_time_heap_count_available__matches_init);
    RUN_TEST(test_one_time_heap_count_available__matches_init_empty);

    RUN_TEST(test_one_time_heap_disable);

    RUN_TEST(test_one_time_heap_alloc);
    RUN_TEST(test_one_time_heap_alloc__zero__fails);
    RUN_TEST(test_one_time_heap_alloc__too_much__fails);
    RUN_TEST(test_one_time_heap_alloc__disabled__fails);
    RUN_TEST(test_one_time_heap_alloc__just_enough__ok);
    RUN_TEST(test_one_time_heap_alloc__aligned_to_8_bytes);
    RUN_TEST(test_one_time_heap_alloc_aligned__aligned_to_n_bytes);

    UNITY_END();

    return 0;
}
