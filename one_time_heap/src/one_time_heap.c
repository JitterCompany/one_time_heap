#include "one_time_heap.h"

void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory)
{
    heap->start = memory;
    heap->next_alloc = memory;
    heap->end = memory + sizeof_memory;
}

