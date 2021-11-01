#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/keyword.h"
#include <stdlib.h>

MCL_STDC_BEGIN

#ifndef MCL_MEMORY_FORCE_CLEAN
#define MCL_MEMORY_FORCE_CLEAN
#endif

MCL_INLINE void* mcl_malloc(uint32_t size) {
    void* p = malloc(size);
#ifdef MCL_MEMORY_FORCE_CLEAN
    memset(p, 0, size);
#endif
    return p;
}

MCL_INLINE void mcl_free(void * p) {
    if (p) {
        free(p);
    }
}

MCL_INLINE void mcl_auto_free(void* pp) {
    if (!pp) return;
    void** p_pointer =  (void**)pp;
    mcl_free(*p_pointer);
    (*p_pointer) = NULL;
}

#define MCL_MALLOC(SIZE)    mcl_malloc(SIZE)

#define MCL_FREE(PTR)                   \
do {                                    \
    mcl_free(PTR);                      \
    (PTR) = NULL;                       \
} while(0)

#define MCL_AUTO_PTR    MCL_RAII(mcl_auto_free)

MCL_STDC_END

#endif