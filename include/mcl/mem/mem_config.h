#ifndef H6599804E_7086_4E84_B92C_6AA7429F00FD
#define H6599804E_7086_4E84_B92C_6AA7429F00FD

#include "mcl/config.h"
#include "mcl/stdtype.h"

#ifdef MCL_CONFIG_MEM

MCL_STDC_BEGIN

void* MclMem_Malloc(uint32_t size);
void  MclMem_Free(void*);

MCL_STDC_END

#define MCL_MEM_MALLOC(size)   MclMem_Malloc((size))
#define MCL_MEM_FREE(pointer)  MclMem_Free((pointer))

#else

#include <stdlib.h>

#define MCL_MEM_MALLOC(size)   malloc((size))
#define MCL_MEM_FREE(pointer)  free((pointer))

#endif

#endif