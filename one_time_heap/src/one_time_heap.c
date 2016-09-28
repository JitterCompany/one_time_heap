#include "one_time_heap.h"
#include <string.h>
#include <c_utils/align.h>

#define ALIGNMENT 8

void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory)
{
    memset(memory, 0, sizeof_memory);

    heap->end = ((uint8_t*)memory) + sizeof_memory;
    heap->next_alloc = align(memory, ALIGNMENT);
    if(heap->next_alloc > heap->end) {
        heap->next_alloc = heap->end;
    }
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
    heap->next_alloc = align((heap->next_alloc + num_bytes), ALIGNMENT);
    if(heap->next_alloc > heap->end) {
        heap->next_alloc = heap->end;
    }
    return result;
}

