#ifndef H81102F3A_EA60_4478_8DDB_B6A20D88388C
#define H81102F3A_EA60_4478_8DDB_B6A20D88388C

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/domain/value/mcl_integer.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

extern const size_t MCL_ENTITY_SIZE;

MclStatus MclEntity_Init(MclEntity*, MclEntityId);
void MclEntity_Destroy(MclEntity*);

MclEntityId MclEntity_GetId(const MclEntity*);
MclInteger  MclEntity_GetValue(const MclEntity*);
MclAggregatorId MclEntity_GetAggregatorId(const MclEntity*);

void MclEntity_OnInsertToAggregator(MclEntity*, MclAggregatorId);
void MclEntity_OnRemoveFromAggregator(MclEntity*);

MclStatus   MclEntity_DoubleValue(MclEntity*);
void MclEntity_ClearValue(MclEntity*);

MCL_STDC_END

#endif
