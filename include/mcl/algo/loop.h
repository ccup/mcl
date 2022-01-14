#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A2
#define H90B6970A_7AC3_43F4_9432_0647CA0925A2

#include "mcl/stdtype.h"

MCL_STDC_BEGIN

#define MCL_LOOP_FOREVER()  while(true)

#define MCL_LOOP_FOREACH(type, i, begin, end, step)   	\
	for(type i = begin; i < end; i += step)

#define MCL_LOOP_FOREACH_SIZE(i, max) 					\
	MCL_LOOP_FOREACH(MclSize, i, 0, max, 1)

#define MCL_LOOP_FOREACH_STEP(i, max, step)				\
	MCL_LOOP_FOREACH(MclSize, i, 0, max, step)

MCL_STDC_END

#endif
