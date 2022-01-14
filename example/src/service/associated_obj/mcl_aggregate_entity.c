#include "associated_obj/mcl_aggregate_entity.h"
#include "mcl/assert.h"
#include "mcl/lock/lockobj.h"

//#pragma GCC diagnostic ignored "-Wunused-variable"
MCL_STATIC_ASSERT(sizeof(MclAggregateEntity) == sizeof(MclAggregateEntityConst));

void MclAggregateEntity_WrUnlock(MclAggregateEntity *aggregateEntity) {
    MCL_ASSERT_VALID_PTR_VOID(aggregateEntity);
    if (aggregateEntity->aggregator == NULL) return;
    MCL_ASSERT_SUCC_CALL_VOID(MclLockObj_UnLock(aggregateEntity->aggregator));
}

void MclAggregateEntity_RdUnlock(MclAggregateEntityConst *aggregateEntity) {
    MCL_ASSERT_VALID_PTR_VOID(aggregateEntity);
    if (aggregateEntity->aggregator == NULL) return;
    MCL_ASSERT_SUCC_CALL_VOID(MclLockObj_UnLock((void*)aggregateEntity->aggregator));
}
