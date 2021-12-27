#ifndef H56A53302_359B_4C7B_996F_2B0CFD849CE9
#define H56A53302_359B_4C7B_996F_2B0CFD849CE9

#include "mcl/typedef.h"

#define MCL_PRIVATE static

#ifndef MCL_STATIC_ASSERT
#define MCL_STATIC_ASSERT(exp)  extern void __mcl_static_assert(int arg[(exp) ? 1 : -1])
#endif

#ifdef __GNUC__
	#define MCL_CTOR            __attribute__((constructor))
	#define MCL_DTOR            __attribute__((destructor))
	#define MCL_PLACEHOLDER     __attribute__ ((weak))
    #define MCL_RAII(function)	__attribute__((cleanup(function)))
    #define MCL_MALLOC_API      __attribute__ ((malloc))
    #define MCL_PURE            __attribute__ ((pure))
    #define MCL_INLINE          __attribute__ ((always_inline)) inline
#else
    #define MCL_CTOR            MCL_STATIC_ASSERT("MCL CTOR NOT SUPPORTED!")
	#define MCL_DTOR            MCL_STATIC_ASSERT("MCL DTOR NOT SUPPORTED!")
	#define MCL_PLACEHOLDER     MCL_STATIC_ASSERT("MCL PLACEHOLDER NOT SUPPORTED!")
    #define MCL_RAII(function)  MCL_STATIC_ASSERT("MCL RAII NOT SUPPORTED!")
    #define MCL_MALLOC_API      MCL_STATIC_ASSERT("MCL MALLOC API NOT SUPPORTED!")
    #define MCL_PURE            MCL_STATIC_ASSERT("MCL PURE NOT SUPPORTED!")
    #define MCL_INLINE          static inline
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_MOD
        #ifdef __GNUC__
            #define MCL_PUBLIC __attribute__ ((dllexport))
        #else
            #define MCL_PUBLIC __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define MCL_PUBLIC __attribute__ ((dllimport))
        #else
            #define MCL_PUBLIC __declspec(dllimport)
        #endif
    #endif
    #define MCL_LOCAL
#else
    #if __GNUC__ >= 4
        #define MCL_PUBLIC __attribute__ ((visibility ("default")))
        #define MCL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define MCL_PUBLIC
        #define MCL_LOCAL
    #endif
#endif

#endif
