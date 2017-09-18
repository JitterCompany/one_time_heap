#include "one_time_heap.h"
#include <string.h>
#include <c_utils/align.h>


void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory)
{
    memset(memory, 0, sizeof_memory);

    heap->end = ((uint8_t*)memory) + sizeof_memory;
    heap->next_available = memory;
}

size_t one_time_heap_count_available(OneTimeHeap *heap, size_t alignment)
{
    uint8_t *aligned = align(heap->next_available, alignment);
    if(aligned >= heap->end) {
        return 0;
    }
    return (heap->end - aligned);
}

void one_time_heap_disable(OneTimeHeap *heap)
{
    heap->next_available = heap->end;
}

void *one_time_heap_alloc(OneTimeHeap *heap, size_t num_bytes)
{
    return one_time_heap_alloc_aligned(heap, num_bytes,
            ONE_TIME_HEAP_DEFAULT_ALIGNMENT);
}

void *one_time_heap_alloc_aligned(OneTimeHeap *heap,
        size_t num_bytes, size_t alignment)
{
    size_t available = one_time_heap_count_available(heap, alignment);

    if((!num_bytes) || (available < num_bytes)) {
        return NULL;
    }
    void *result = align(heap->next_available, alignment);
    heap->next_available = ((uint8_t*)result) + num_bytes;

    return result;
}

