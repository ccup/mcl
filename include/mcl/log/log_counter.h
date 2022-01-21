#ifndef HF9533ABB_39C2_4950_A780_136743417F49
#define HF9533ABB_39C2_4950_A780_136743417F49

#include "mcl/log/log_level.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

void MclLogCounter_CountLevel(MclLogLevel);

MclSize MclLogCounter_GetFatalCount();
MclSize MclLogCounter_GetErrorCount();
MclSize MclLogCounter_GetWarnCount();

MCL_STDC_END

#endif
