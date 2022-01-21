#ifndef HA2D941FC_03EB_4ED7_9CD2_E9274142BAA7
#define HA2D941FC_03EB_4ED7_9CD2_E9274142BAA7

#include "mcl/stdtype.h"

MCL_STDC_BEGIN

void MclMemCounter_CountMalloc();
void MclMemCounter_CountFree();

MclSize MclMemCounter_GetMallocCount();
MclSize MclMemCounter_GetFreeCount();

MCL_STDC_END

#endif
