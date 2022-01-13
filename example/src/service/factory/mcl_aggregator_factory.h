#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF025
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF025

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);

size_t MclAggregatorFactory_GetUnreleasedCount();

MclAggregator* MclAggregatorFactory_Create(MclAggregatorId);
void MclAggregatorFactory_Delete(MclAggregator*);

MclAggregator* MclAggregatorFactory_CreateSharedPtr(MclAggregatorId);
void MclAggregatorFactory_DeleteSharedPtr(MclAggregator*);

MclAggregator* MclAggregatorFactory_CreateLockObj(MclAggregatorId);
void MclAggregatorFactory_DeleteLockObj(MclAggregator*);

MclAggregator* MclAggregatorFactory_CreateStatic(MclAggregatorId);
void MclAggregatorFactory_DeleteStatic(MclAggregator*);

MCL_STDC_END

#endif
