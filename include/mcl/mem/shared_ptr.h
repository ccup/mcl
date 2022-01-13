#ifndef MCL_46F99D91864B4D238E0DC000FDEC7E31
#define MCL_46F99D91864B4D238E0DC000FDEC7E31

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef void (*MclSharedPtrDestroy)(void *ptr, void *arg);

void* MclSharedPtr_Create(size_t, MclSharedPtrDestroy, void *arg);

void  MclSharedPtr_Delete(void *ptr);

void* MclSharedPtr_Ref(void *ptr);

///////////////////////////////////////////////////////////
MCL_INLINE void MclSharedPtr_AutoFree(void *pp) {
    if (!pp) return;
    void** p_pointer =  (void**)pp;
    if (*p_pointer) MclSharedPtr_Delete(*p_pointer);
    (*p_pointer) = NULL;
}

#define MCL_SHARED_AUTO    MCL_RAII(MclSharedPtr_AutoFree)

#define MCL_SHARED_REF(Type, ptr) (Type*)MclSharedPtr_Ref(ptr)

#define MCL_SHARED_PTR(Type, ...)                                           \
    ({                                                                      \
        struct MclSharedTmp {                                               \
            __typeof__(Type) value;                                         \
            MclSharedPtrDestroy destroy;                                    \
            void *arg;                                                      \
        } args = {                                                          \
            __VA_ARGS__                                                     \
        };                                                                  \
        void *ptr = MclSharedPtr_Create(sizeof(Type), args.destroy, args.arg); \
        if (ptr != NULL) memcpy(ptr, &args.value, sizeof(Type));            \
        (Type*)ptr;                                                         \
    })

MCL_STDC_END

#endif
