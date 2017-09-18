#ifndef ONE_TIME_HEAP_H
#define ONE_TIME_HEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *next_available;
    uint8_t *end;
} OneTimeHeap;

#define ONE_TIME_HEAP_DEFAULT_ALIGNMENT 8

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
 *
 * @param alignment Alignment for next pointer to be requested. If you want to
 *                  allocate memory aligned to 64 bytes, less space may be 
 *                  available than if you would request a buffer aligned
 *                  to 4 bytes.
 *
 * @return          Amount of bytes available for the given alignment
 */
size_t one_time_heap_count_available(OneTimeHeap *heap, size_t alignment);

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
 *                      The buffer is guaranteed to be at least 8-byte aligned.
 *                      NULL is returned if not enough space is available
 *                      or if num_bytes is 0.
 */
void *one_time_heap_alloc(OneTimeHeap *heap, size_t num_bytes);

/**
 * Try to allocate aligned memory.
 * Similar to one_time_heap_alloc(), but with user-defined alignment.
 * NOTE: alignment should be a power of two!
 *
 * @param heap          Ptr to a OneTimeHeap instance previously initialized
 *                      with one_time_heap_init().
 *
 * @param num_bytes     Amount of bytes requested.
 *
 * @param alignment     The result is aligned to this amount of bytes.
 *                      NOTE: this value should be a power of two.
 *
 * @return              Pointer to a buffer of the requested size,
 *                      or NULL if the allocation failed.
 *                      NULL is returned if not enough space is available
 *                      or if num_bytes is 0.
 */
void *one_time_heap_alloc_aligned(OneTimeHeap *heap,
        size_t num_bytes, size_t alignment);
#endif

