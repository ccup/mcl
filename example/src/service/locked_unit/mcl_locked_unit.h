#ifndef H9D2E73D9_46D0_4C0D_A563_2D6D5D085980
#define H9D2E73D9_46D0_4C0D_A563_2D6D5D085980

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/domain/entity/mcl_entity_id.h"
#include "repo/mcl_aggregator_repo.h"
#include "repo/mcl_entity_repo.h"
#include "mcl/lock/lockobj.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);
MCL_TYPE_DECL(MclEntity);

MCL_TYPE(MclLockedAggregatorEntity) {
	MclAggregator *aggregator;
	MclEntity *entity;
};

MCL_TYPE(MclLockedAggregatorEntityConst) {
	const MclAggregator *aggregator;
	const MclEntity *entity;
};

///////////////////////////////////////////////////////////
void MclLockedUnit_LockAggregatorEntity(MclLockedAggregatorEntity*, MclEntityId);
void MclLockedUnit_LockAggregatorEntityConst(MclLockedAggregatorEntityConst*, MclEntityId);

///////////////////////////////////////////////////////////
void MclLockedUnit_UnlockAggregatorEntity(MclLockedAggregatorEntity*);
void MclLockedUnit_UnlockAggregatorEntityConst(MclLockedAggregatorEntityConst*);

///////////////////////////////////////////////////////////
#define MCL_UNLOCK_AGGREGATOR_ENTITY_AUTO        MCL_RAII(MclLockedUnit_UnlockAggregatorEntity)
#define MCL_UNLOCK_AGGREGATOR_ENTITY_CONST_AUTO  MCL_RAII(MclLockedUnit_UnlockAggregatorEntityConst)

///////////////////////////////////////////////////////////
#define MCL_LOCK_AGGREGATOR(AGG, AGG_ID)						\
MCL_LOCK_OBJ_AUTO MclAggregator *AGG = MclAggregatorRepo_Fetch(AGG_ID)

#define MCL_LOCK_AGGREGATOR_CONST(AGG, AGG_ID)					\
MCL_LOCK_OBJ_AUTO const MclAggregator *AGG = MclAggregatorRepo_FetchConst(AGG_ID)

#define MCL_LOCK_AGGREGATOR_BY(AGG, ...)						\
MCL_LOCK_OBJ_AUTO MclAggregator *AGG = MclAggregatorRepo_FetchBy(__VA_ARGS__)

#define MCL_LOCK_AGGREGATOR_CONST_BY(AGG, ...)					\
MCL_LOCK_OBJ_AUTO const MclAggregator *AGG = MclAggregatorRepo_FetchConstBy(__VA_ARGS__)

#define MCL_LOCK_ENTITY(ENTITY, ENTITY_ID)						\
MCL_LOCK_OBJ_AUTO MclEntity *ENTITY = MclEntityRepo_Fetch(ENTITY_ID)

#define MCL_LOCK_ENTITY_CONST(ENTITY, ENTITY_ID)				\
MCL_LOCK_OBJ_AUTO const MclEntity *ENTITY = MclEntityRepo_FetchConst(ENTITY_ID)

#define MCL_LOCK_ENTITY_BY(ENTITY, ...)							\
MCL_LOCK_OBJ_AUTO MclEntity *ENTITY = MclEntityRepo_FetchBy(__VA_ARGS__)

#define MCL_LOCK_ENTITY_CONST_BY(ENTITY, ...)					\
MCL_LOCK_OBJ_AUTO const MclEntity *ENTITY = MclEntityRepo_FetchConstBy(__VA_ARGS__)

#define MCL_LOCK_AGGREGATOR_ENTITY(ENTITY, ENTITY_ID)			\
MCL_UNLOCK_AGGREGATOR_ENTITY_AUTO MclLockedAggregatorEntity ENTITY = {0};	\
MclLockedUnit_LockAggregatorEntity(&ENTITY, ENTITY_ID)

#define MCL_LOCK_AGGREGATOR_ENTITY_CONST(ENTITY, ENTITY_ID)		\
MCL_UNLOCK_AGGREGATOR_ENTITY_CONST_AUTO MclLockedAggregatorEntityConst ENTITY = {0};	\
MclLockedUnit_LockAggregatorEntityConst(&ENTITY, ENTITY_ID)

MCL_STDC_END

#endif
