#ifndef H29D57B96_5262_4A29_9D37_7961086904A7
#define H29D57B96_5262_4A29_9D37_7961086904A7

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);
MCL_TYPE_DECL(MclEntity);

extern const size_t MCL_AGGREGATOR_SIZE;

MclStatus MclAggregator_Init(MclAggregator*, MclAggregatorId);
void MclAggregator_Destroy(MclAggregator*);

MclAggregatorId MclAggregator_GetId(const MclAggregator*);
size_t MclAggregator_GetEntityCount(const MclAggregator*);

MclStatus MclAggregator_AddEntity(MclAggregator*, MclEntity*);
void MclAggregator_RemoveEntity(MclAggregator*, MclEntity*);

MclStatus MclAggregator_DoubleAll(MclAggregator*);

MCL_STDC_END

#endif
