#ifndef H56A53302_359B_4C7B_996F_2B0CFD849CE9
#define H56A53302_359B_4C7B_996F_2B0CFD849CE9

#include "mcl/stdc.h"
#include "mcl/static_assert.h"

#define MCL_PRIVATE static

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

#ifdef __GNUC__
	#define MCL_PLACEHOLDER __attribute__ ((weak))
#else
	#define MCL_PLACEHOLDER
#endif

#ifdef __GNUC__
	#define MCL_RAII(function, ...)	__attribute__((cleanup(function)))  __VA_ARGS__
#else
	#define MCL_RAII(function, ...) STATIC_ASSERT("MCL RAII NOT SUPPORT!")
#endif

#ifdef __GNUC__
	#define MCL_CTOR __attribute__((constructor))
	#define MCL_DTOR __attribute__((destructor))
#else
	#define MCL_CTOR STATIC_ASSERT("MCL CTOR NOT SUPPORT!")
	#define MCL_DTOR STATIC_ASSERT("MCL DTOR NOT SUPPORT!")
#endif

#endif
