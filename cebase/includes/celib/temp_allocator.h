//==============================================================================
// Based on bitsquid foundation.
// git+web: https://bitbucket.org/bitsquid/foundation
//==============================================================================

#ifndef CELIB_TEMP_ALLOCATOR_H
#define CELIB_TEMP_ALLOCATOR_H

//==============================================================================
// Includes
//==============================================================================

#include "memory.h"
#include "types.h"

//==============================================================================
// Interface macro
//==============================================================================

#define TEMP_ALLOCATOR_DEF(S)                                                  \
struct temp_allocator_##S {                                                    \
    struct cel_allocator base;                                                     \
    char _buffer[S];                                                           \
    struct cel_allocator *_backing;                                                \
    char *_start;                                                              \
    char *_p;                                                                  \
    char *_end;                                                                \
    unsigned _chunk_size;                                                      \
};                                                                             \
                                                                               \
static void *temp_allocator_allocate_##S(struct cel_allocator *allocator,          \
                                         uint32_t size, uint32_t align) {      \
    struct temp_allocator_##S *a = (struct temp_allocator_##S *)allocator;     \
                                                                               \
    a->_p = (char *) pointer_align_forward(a->_p, align);                      \
    if ((int) size > a->_end - a->_p) {                                        \
        uint32_t to_allocate = sizeof(void *) + size + align;                  \
        if (to_allocate < a->_chunk_size)                                      \
            to_allocate = a->_chunk_size;                                      \
                                                                               \
        a->_chunk_size *= 2;                                                   \
                                                                               \
        void *p = CEL_ALLOCATE(a->_backing, to_allocate);                       \
        *(void **) a->_start = p;                                              \
        a->_p = a->_start = (char *) p;                                        \
        a->_end = a->_start + to_allocate;                                     \
        *(void **) a->_start = 0;                                              \
        a->_p += sizeof(void *);                                               \
        a->_p = (char *) pointer_align_forward(a->_p, align);                  \
    }                                                                          \
                                                                               \
    void *result = a->_p;                                                      \
    a->_p += size;                                                             \
    return result;                                                             \
}                                                                              \
                                                                               \
static void temp_allocator_create_##S(struct temp_allocator_64 *allocator,     \
                                      struct cel_allocator *backing) {             \
                                                                               \
    allocator->_backing = backing;                                             \
    allocator->_chunk_size = _4KiB;                                            \
                                                                               \
    allocator->_p = allocator->_start = allocator->_buffer;                    \
    allocator->_end = allocator->_start + 64;                                  \
    *(void **) allocator->_start = 0;                                          \
    allocator->_p += sizeof(void *);                                           \
                                                                               \
    allocator->base = {                                                        \
            .allocate = temp_allocator_allocate_##S,                           \
            .deallocate = _temp_allocator_deallocate,                          \
            .allocated_size = _temp_allocator_allocated_size,                  \
            .total_allocated = _temp_allocator_total_allocated                 \
    };                                                                         \
}                                                                              \
                                                                               \
static void temp_allocator_destroy_##S(struct temp_allocator_64 *allocator) {  \
    void *p = *(void **) allocator->_buffer;                                   \
    while (p) {                                                                \
        void *next = *(void **) p;                                             \
                                                                               \
        CEL_DEALLOCATE(allocator->_backing, p);                                 \
                                                                               \
        p = next;                                                              \
    }                                                                          \
}                                                                              \


static void _temp_allocator_deallocate(struct cel_allocator *allocator,
                                       void *p) {
}

static uint32_t _temp_allocator_total_allocated(struct cel_allocator *allocator) {
    return CEL_SIZE_NOT_TRACKED;
}

static uint32_t _temp_allocator_allocated_size(void *p) {
    return CEL_SIZE_NOT_TRACKED;
}

//==============================================================================
// Predifend allocator size
//==============================================================================

TEMP_ALLOCATOR_DEF(64)

TEMP_ALLOCATOR_DEF(128)

TEMP_ALLOCATOR_DEF(256)

TEMP_ALLOCATOR_DEF(512)

TEMP_ALLOCATOR_DEF(1024)

TEMP_ALLOCATOR_DEF(2048)

TEMP_ALLOCATOR_DEF(4096)

#endif //CELIB_TEMP_ALLOCATOR_H