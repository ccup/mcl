#ifndef HA3A85B0B_C344_408F_B1D3_68E5BA76C844
#define HA3A85B0B_C344_408F_B1D3_68E5BA76C844

#include "mcl/config.h"
#include "mcl/time/time_type.h"

#ifdef MCL_CONFIG_TIME

MCL_STDC_BEGIN

MclTimeSec MclTime_GetCurrentUs();
MclTimeMs  MclTime_GetCurrentMs();
MclTimeUs  MclTime_GetCurrentUs();
MclTimeNs  MclTime_GetCurrentNs();

void MclTime_DelaySec(MclTimeSecDiff);
void MclTime_DelayMs(MclTimeMsDiff);
void MclTime_DelayUs(MclTimeUsDiff);
void MclTime_DelayNs(MclTimeNsDiff);

MCL_STDC_END

#define MCL_TIME_SEC_NOW()   MclTime_GetCurrentSec()
#define MCL_TIME_MS_NOW()    MclTime_GetCurrentMs()
#define MCL_TIME_US_NOW()    MclTime_GetCurrentUs()
#define MCL_TIME_NS_NOW()    MclTime_GetCurrentNs()

#define MCL_TIME_SEC_DELAY(SEC_DIFF)  MclTime_DelaySec(SEC_DIFF)
#define MCL_TIME_MS_DELAY(MS_DIFF)    MclTime_DelayMs(MS_DIFF)
#define MCL_TIME_US_DELAY(US_DIFF)    MclTime_DelayUs(US_DIFF)
#define MCL_TIME_NS_DELAY(NS_DIFF)    MclTime_DelayNs(NS_DIFF)

#else

#include <unistd.h>

#define MCL_TIME_SEC_NOW() MCL_TIME_SEC_INVALID
#define MCL_TIME_MS_NOW()  MCL_TIME_MS_INVALID
#define MCL_TIME_US_NOW()  MCL_TIME_US_INVALID
#define MCL_TIME_NS_NOW()  MCL_TIME_NS_INVALID

#define MCL_TIME_SEC_DELAY(SEC_DIFF)   (void)sleep(SEC_DIFF)
#define MCL_TIME_MS_DELAY(MS_DIFF)     (void)usleep((MS_DIFF) * 10)
#define MCL_TIME_US_DELAY(US_DIFF)     (void)usleep(US_DIFF)
#define MCL_TIME_NS_DELAY(NS_DIFF)     (void)0/*Unsupport ns time*/

#endif

#endif
