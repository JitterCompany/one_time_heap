#ifndef ONE_TIME_HEAP_H
#define ONE_TIME_HEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *next_alloc;
    uint8_t *end;
} OneTimeHeap;

/**
 * Initialize a OneTimeHeap instance
 *
 * @param heap          Ptr to a OneTimeHeap instance.
 *
 * @param memory        Ptr to a memory buffer. Each one_time_heap_alloc() call
 *                      uses a portion of this memory, untill everything is used.
 *                      Note: make sure this buffer does not go out of scope.
 *                      It is recommended to use static / global memory.
 *
 * @param sizeof_memory Size of the memory buffer in bytes
 *
 */ 
void one_time_heap_init(OneTimeHeap *heap, void *memory, size_t sizeof_memory);

/**
 * Count how many bytes are available.
 * Note: calling one_time_heap_disable() causes the available cout to be zero.
 *
 * @param heap      Ptr to a OneTimeHeap instance previously initialized
 *                  with one_time_heap_init().
 */
size_t one_time_heap_count_available(OneTimeHeap *heap);

/**
 * Disable any future allocs (untill one_time_heap_init() is called again).
 * After calling this function, no more bytes are available.
 *
 * @param heap      Ptr to a OneTimeHeap instance previously initialized
 *                  with one_time_heap_init().
 */
void one_time_heap_disable(OneTimeHeap *heap);

/**
 * Try to allocate the specified amount of bytes
 *
 * @param heap          Ptr to a OneTimeHeap instance previously initialized
 *                      with one_time_heap_init().
 *
 * @param num_bytes     Amount of bytes requested.
 *
 * @return              Pointer to a buffer of the requested size,
 *                      or NULL if the allocation failed.
 *                      NULL is returned if not enough space is available
 *                      or if num_bytes is 0.
 */
void *one_time_heap_alloc(OneTimeHeap *heap, size_t num_bytes);

#endif

