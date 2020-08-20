#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/stdc.h"
#include <stddef.h>

MCL_STDC_BEGIN

#define MCL_FOR_EACH(type, i, begin, end)   for(type i = begin; i< end; ++i)

#define MCL_FOR_EACH_INDEX(i, max) MCL_FOR_EACH(size_t, i, 0, max)

#define MCL_FOREVER()  while(true)

MCL_STDC_END

#endif