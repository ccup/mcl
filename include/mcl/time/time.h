#ifndef HEE4A9B08_1288_4766_B1C2_197732D8818D
#define HEE4A9B08_1288_4766_B1C2_197732D8818D

#include "mcl/time/time_config.h"
#include "mcl/thread/thread.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_INLINE MclTimeSec MclTimeSec_GetNow() {
	return MCL_TIME_SEC_NOW();
}

MCL_INLINE void MclTimeSec_Delay(MclTimeSecDiff diff) {
	MCL_TIME_SEC_DELAY(diff);
}

MCL_INLINE MclTimeMs MclTimeMs_GetNow() {
	return MCL_TIME_MS_NOW();
}

MCL_INLINE void MclTimeMs_Delay(MclTimeMsDiff diff) {
	MCL_TIME_MS_DELAY(diff);
}

MCL_INLINE MclTimeUs MclTimeUs_GetNow() {
	return MCL_TIME_US_NOW();
}

MCL_INLINE void MclTimeUs_Delay(MclTimeUsDiff diff) {
	MCL_TIME_US_DELAY(diff);
}

MCL_INLINE MclTimeNs MclTimeNs_GetNow() {
	return MCL_TIME_NS_NOW();
}

MCL_INLINE void MclTimeNs_Delay(MclTimeNsDiff diff) {
	MCL_TIME_NS_DELAY(diff);
}

MCL_STDC_END

#endif
