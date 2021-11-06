#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A2
#define H90B6970A_7AC3_43F4_9432_0647CA0925A2

#include "mcl/stdc.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

#define MCL_LOOP_FOREACH(type, i, begin, end)   for(type i = begin; i< end; ++i)

#define MCL_LOOP_FOREACH_INDEX(i, max) MCL_LOOP_FOREACH(uint32_t, i, 0, max)

#define MCL_LOOP_FOREVER()  while(true)

MCL_STDC_END

#endif
