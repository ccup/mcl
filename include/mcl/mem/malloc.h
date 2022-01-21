#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/mem/mem_config.h"
#include "mcl/mem/mem_counter.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

///////////////////////////////////////////////////////////
MCL_INLINE MCL_MALLOC_API void* Mcl_Malloc(MclSize size) {
    void* p = MCL_MEM_MALLOC(size);
    if (p) {
#if MCL_CONFIG_MEM_FORCE_CLEAN
    memset(p, 0, size);
#endif
#if MCL_CONFIG_MEM_COUNT_ENABLE
    MclMemCounter_CountMalloc();
#endif
    }
    return p;
}

MCL_INLINE void Mcl_Free(void * p) {
    if (p) {
    	MCL_MEM_FREE(p);
#if MCL_CONFIG_MEM_COUNT_ENABLE
    	MclMemCounter_CountFree();
#endif
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
