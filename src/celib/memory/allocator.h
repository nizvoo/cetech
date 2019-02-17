//                  **Abstract memory alocator**
//

#ifndef CE_MEMORY_ALLOCATOR_H
#define CE_MEMORY_ALLOCATOR_H


#ifdef __cplusplus
extern "C" {
#endif

#include "celib/celib_types.h"


#define CE_ALLOC(a, T, size)                        \
    (T*)((a)->vt->reallocate((a->inst),             \
                               NULL,                \
                               size,                \
                               CE_ALIGNOF(T),       \
                               __FILE__,            \
                               __LINE__))

#define CE_ALLOCATE_ALIGN(a, T, size, align)        \
    (T*)((a)->vt->reallocate((a),                   \
                               NULL,                \
                               size,                \
                               align,               \
                               __FILE__,            \
                               __LINE__))

#define CE_FREE(a, p) \
    ((a)->vt->reallocate((a->inst),p,0,0, __FILE__, __LINE__))

typedef struct ce_alloc_o0 ce_alloc_o0;

typedef struct ce_alloc_vt0 {
    void *(*reallocate)(const ce_alloc_o0 *a,
                        void *ptr,
                        uint32_t size,
                        uint32_t align,
                        const char *filename,
                        uint32_t line);
} ce_alloc_vt0;

typedef struct ce_alloc_t0 {
    ce_alloc_o0 *inst;
    ce_alloc_vt0 *vt;
} ce_alloc_t0;

#ifdef __cplusplus
};
#endif

#endif //CE_MEMORY_ALLOCATOR_H