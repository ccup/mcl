#ifndef H6599804E_7086_4E84_B92C_6AA7429F00FD
#define H6599804E_7086_4E84_B92C_6AA7429F00FD

#include "mcl/config.h"
#include "mcl/stdtype.h"

#ifndef MCL_CONFIG_MEM_MALLOC_CLEAN_ENABLE
#define MCL_CONFIG_MEM_MALLOC_CLEAN_ENABLE 1
#endif

#ifndef MCL_CONFIG_MEM_COUNT_ENABLE
#define MCL_CONFIG_MEM_COUNT_ENABLE 1
#endif

#ifdef MCL_CONFIG_MEM

MCL_STDC_BEGIN

void* MclMem_Malloc(MclSize size);
void  MclMem_Free(void *p);
void* MclMem_Clear(void *p, MclSize size);
void* MclMem_Copy(void *dst, void *src, MclSize size);

MCL_STDC_END

#define MCL_MEM_MALLOC(size)   			MclMem_Malloc((size))
#define MCL_MEM_FREE(pointer)  			MclMem_Free((pointer))
#define MCL_MEM_CLEAR(pointer, size)	MclMem_Clear((pointer), (size))
#define MCL_MEM_COPY(dst, src, size)	MclMem_Copy((dst), (src), (size))

#else

#include <stdlib.h>

#define MCL_MEM_MALLOC(size)            malloc((size))
#define MCL_MEM_FREE(pointer)  			free((pointer))
#define MCL_MEM_CLEAR(pointer, size)	memset((pointer), 0, (size))
#define MCL_MEM_COPY(dst, src, size)	memcpy((dst), (src), (size))

#endif

#endif
