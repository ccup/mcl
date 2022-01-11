#ifndef H56A53302_359B_4C7B_996F_2B0CFD849CE9
#define H56A53302_359B_4C7B_996F_2B0CFD849CE9

#include "mcl/typedef.h"

#define MCL_PRIVATE static
#define __MCL_INLINE static inline

#if __GNUC__ >= 4
    #define MCL_INLINE          __attribute__ ((always_inline)) __MCL_INLINE
	#define MCL_CTOR            __attribute__ ((constructor))
	#define MCL_DTOR            __attribute__ ((destructor))
    #define MCL_RAII(function)	__attribute__ ((cleanup(function)))
	#define MCL_PLACEHOLDER     __attribute__ ((weak))
    #define MCL_MALLOC_API      __attribute__ ((malloc))
    #define MCL_PURE            __attribute__ ((pure))
    #define MCL_UNUSED          __attribute__ ((unused))
    #define MCL_PUBLIC          __attribute__ ((visibility ("default")))
    #define MCL_LOCAL           __attribute__ ((visibility ("hidden")))
#else
    #define MCL_INLINE          __MCL_INLINE
    #define MCL_CTOR
	#define MCL_DTOR
	#define MCL_PLACEHOLDER
    #define MCL_RAII(function)
    #define MCL_MALLOC_API
    #define MCL_PURE
    #define MCL_UNUSED
    #define MCL_PUBLIC
    #define MCL_LOCAL
#endif

#endif
