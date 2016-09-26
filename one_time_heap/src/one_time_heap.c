#include "one_time_heap.h"
#include <string.h>

void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory)
{
    heap->next_alloc = memory;
    heap->end = ((uint8_t*)memory) + sizeof_memory;

    memset(memory, 0, sizeof_memory);
}

size_t one_time_heap_count_available(OneTimeHeap *heap)
{
    return (heap->end - heap->next_alloc);
}

void one_time_heap_disable(OneTimeHeap *heap)
{
    heap->next_alloc = heap->end;
}

void *one_time_heap_alloc(OneTimeHeap *heap, size_t num_bytes)
{
    size_t available = one_time_heap_count_available(heap);

    if((!num_bytes) || (available < num_bytes)) {
        return NULL;
    }
    void *result = heap->next_alloc;
    heap->next_alloc+=num_bytes;
    return result;
}

