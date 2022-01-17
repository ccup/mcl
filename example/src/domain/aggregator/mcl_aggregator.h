#ifndef H29D57B96_5262_4A29_9D37_7961086904A7
#define H29D57B96_5262_4A29_9D37_7961086904A7

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/value/mcl_integer.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);
MCL_TYPE_DECL(MclEntity);

extern const MclSize MCL_AGGREGATOR_SIZE;

MclStatus MclAggregator_Init(MclAggregator*, MclAggregatorId, void *cfg);
void MclAggregator_Destroy(MclAggregator*);

MclAggregatorId MclAggregator_GetId(const MclAggregator*);
MclSize MclAggregator_GetEntityCount(const MclAggregator*);

MclStatus MclAggregator_AddEntity(MclAggregator*, MclEntity*);
MclStatus MclAggregator_RemoveEntity(MclAggregator*, MclEntityId);
bool MclAggregator_HasEntity(const MclAggregator*, MclEntityId);

MclStatus MclAggregator_DoubleEntities(MclAggregator*);
MclInteger MclAggregator_GetSumValue(const  MclAggregator*);

MCL_STDC_END

#endif
