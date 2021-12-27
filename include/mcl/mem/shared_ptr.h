#ifndef MCL_46F99D91864B4D238E0DC000FDEC7E31
#define MCL_46F99D91864B4D238E0DC000FDEC7E31

#include "mcl/typedef.h"

MCL_STDC_BEGIN

typedef void (*MclSharedPtrDestructor)(void *ptr, void *arg);

void* MclSharedPtr_Create(size_t size, MclSharedPtrDestructor);
void  MclSharedPtr_Delete(void *ptr);
void* MclSharedPtr_Ref(void *ptr);

MCL_STDC_END

#endif
