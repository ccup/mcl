#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/keyword.h"
#include "mcl/stdtype.h"
#include <stdlib.h>

MCL_STDC_BEGIN

///////////////////////////////////////////////////////////
#ifndef MCL_MALLOC_FORCE_CLEAN
#define MCL_MALLOC_FORCE_CLEAN
#endif

MCL_INLINE MCL_MALLOC_API void* Mcl_Malloc(uint32_t size) {
    void* p = malloc(size);
#ifdef MCL_MALLOC_FORCE_CLEAN
    memset(p, 0, size);
#endif
    return p;
}

MCL_INLINE void Mcl_Free(void * p) {
    if (p) {
        free(p);
    }
}

///////////////////////////////////////////////////////////
#define MCL_MALLOC(SIZE)    Mcl_Malloc(SIZE)

#define MCL_FREE(PTR)                   \
do {                                    \
    Mcl_Free(PTR);                      \
    (PTR) = NULL;                       \
} while(0)

///////////////////////////////////////////////////////////
MCL_INLINE void Mcl_FreeAuto(void* pp) {
    if (!pp) return;
    void** p_pointer =  (void**)pp;
    Mcl_Free(*p_pointer);
    (*p_pointer) = NULL;
}

#define MCL_FREE_AUTO    MCL_RAII(Mcl_FreeAuto)

MCL_STDC_END

#endif
