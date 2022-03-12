#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF025
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF025

#include "mcl/infra/allocator/mcl_allocator_type.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);

void MclAggregatorFactory_Init(MclAllocatorType);

MclAggregator* MclAggregatorFactory_Create(MclAggregatorId, void *cfg);

void MclAggregatorFactory_Delete(MclAggregator*);

MclSize MclAggregatorFactory_GetUnreleasedCount();

MCL_STDC_END

#endif
