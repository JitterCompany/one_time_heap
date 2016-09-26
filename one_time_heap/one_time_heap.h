#ifndef ONE_TIME_HEAP_H
#define ONE_TIME_HEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *start;
    uint8_t *next_alloc;
    uint8_t *end;
} OneTimeHeap;

void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory);
void *one_time_heap_alloc(OneTimeHeap *heap, size_t num_bytes);
void one_time_heap_disable(OneTimeHeap *heap);


#endif

