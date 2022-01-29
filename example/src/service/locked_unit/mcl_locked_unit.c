#include "locked_unit/mcl_locked_unit.h"
#include "aggregator/mcl_aggregator_pred.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MCL_STATIC_ASSERT(sizeof(MclLockedAggregatorEntity) == sizeof(MclLockedAggregatorEntityConst));

MCL_PRIVATE void MclLockedUnit_Unlock(const MclAggregator *aggregator, const MclEntity *entity) {
	if (entity) MCL_ASSERT_SUCC_CALL_VOID(MclLockObj_UnLock((void*)entity));
	if (aggregator) MCL_ASSERT_SUCC_CALL_VOID(MclLockObj_UnLock((void*)aggregator));
}

void MclLockedUnit_UnlockAggregatorEntity(MclLockedAggregatorEntity *lockedUnit) {
    MCL_ASSERT_VALID_PTR_VOID(lockedUnit);
    MclLockedUnit_Unlock(lockedUnit->aggregator, lockedUnit->entity);
}

void MclLockedUnit_UnlockAggregatorEntityConst(MclLockedAggregatorEntityConst *lockedUnit) {
    MCL_ASSERT_VALID_PTR_VOID(lockedUnit);
    MclLockedUnit_Unlock(lockedUnit->aggregator, lockedUnit->entity);
}

void MclLockedUnit_LockAggregatorEntity(MclLockedAggregatorEntity *lockedUnit, MclEntityId entityId)
{
	MCL_ASSERT_VALID_PTR_VOID(lockedUnit);
	MCL_ASSERT_TRUE_VOID(MclEntityId_IsValid(entityId));

	lockedUnit->aggregator = MclAggregatorRepo_FetchBy(MclAggregatorPred_HasEntity, &entityId);
	lockedUnit->entity = MclEntityRepo_Fetch(entityId);
}

void MclLockedUnit_LockAggregatorEntityConst(MclLockedAggregatorEntityConst *lockedUnit, MclEntityId entityId) {
	MCL_ASSERT_VALID_PTR_VOID(lockedUnit);
	MCL_ASSERT_TRUE_VOID(MclEntityId_IsValid(entityId));

	lockedUnit->aggregator = MclAggregatorRepo_FetchConstBy(MclAggregatorPred_HasEntity, &entityId);
	lockedUnit->entity = MclEntityRepo_FetchConst(entityId);
}
