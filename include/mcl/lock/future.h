#ifndef MCL_43D0B2E50B8C433589E03961063D4832
#define MCL_43D0B2E50B8C433589E03961063D4832

#include "mcl/typedef.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclFuture);

MclFuture* MclFuture_Create();
void MclFuture_Delete(MclFuture*);

void MclFuture_Stop(MclFuture*);
bool MclFuture_IsReady(const MclFuture*);

void MclFuture_Set(MclFuture*, MclStatus, void*);
void MclFuture_Get(MclFuture*, MclStatus*, void**);

MCL_STDC_END

#endif
