#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/keyword.h"
#include <stdlib.h>

MCL_STDC_BEGIN

#ifndef MCL_MEMORY_FORCE_CLEAN
#define MCL_MEMORY_FORCE_CLEAN
#endif

MCL_INLINE void* MCL_Malloc(uint32_t size) {
    void* p = malloc(size);
#ifdef MCL_MEMORY_FORCE_CLEAN
    memset(p, 0, size);
#endif
    return p;
}

MCL_INLINE void MCL_Free(void * p) {
    if (p) {
        free(p);
    }
}

MCL_INLINE void MCL_AutoFree(void* pp) {
    if (!pp) return;
    void** p_pointer =  (void**)pp;
    MCL_Free(*p_pointer);
    (*p_pointer) = NULL;
}

#define MCL_MALLOC(SIZE)    MCL_Malloc(SIZE)

#define MCL_FREE(PTR)                   \
do {                                    \
    MCL_Free(PTR);                      \
    (PTR) = NULL;                       \
} while(0)

#define MCL_AUTO_PTR    MCL_RAII(MCL_AutoFree)

MCL_STDC_END

#endif