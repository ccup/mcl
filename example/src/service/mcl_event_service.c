#include "mcl/service/mcl_event_service.h"
#include "repo/mcl_aggregator_repo.h"
#include "repo/mcl_entity_repo.h"
#include "locked_unit/mcl_locked_unit.h"
#include "aggregator/mcl_aggregator.h"
#include "entity/mcl_entity.h"
#include "mcl/lock/lockobj.h"
#include "mcl/defer/defer.h"
#include "mcl/list/list.h"
#include "mcl/assert.h"

typedef struct {
	MclSize count;
} EntityCounter;

MCL_PRIVATE MclStatus MclEventService_CountEntity(const MclEntity *entity, void *arg) {
	EntityCounter *counter = (EntityCounter*)arg;
	MCL_LOG_DBG("Count entity (%u).", MclEntity_GetId(entity));
	counter->count++;
	return MCL_SUCCESS;
}

MclStatus MclEventService_On1sTimeout() {
	EntityCounter counter = {0};
	MCL_ASSERT_SUCC_CALL(MclEntityRepo_AcceptConst(MclEventService_CountEntity, &counter));
	MCL_LOG_SUCC("Event Service: Count (%u) entities on 1S timer OK!", counter.count);
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclEventService_ClearEntity(MclListData data, void *arg) {
	MclEntityId id = (MclEntityId)(data);
	if (id == MCL_SIZE_MAX) id = 0;
	MCL_LOCK_AGGREGATOR_ENTITY(lockedEntity, id);
	if (lockedEntity.entity) {
		MclEntity_ClearValue(lockedEntity.entity);
		MCL_LOG_DBG("Clear entity (%u).", id);
	} else {
		MCL_LOG_DBG("Entity (%u) is releasing when 1S dealing.", id);
	}
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclEventService_CollectEntityIds(const MclEntity *entity, void *arg) {
	MclList *result = (MclList*)arg;
	MclEntityId id = MclEntity_GetId(entity);
	if (id == (MclEntityId)MCL_LIST_DATA_INVALID) {
		id = MCL_SIZE_MAX;
	}
	MCL_ASSERT_VALID_PTR(MclList_PushBack(result, (MclListData)(long)id));
	return MCL_SUCCESS;
}

MCL_PRIVATE void MclEventService_ReleaseList(void *ctxt) {
	MclList *list = (MclList*)ctxt;
	MclList_Delete(list, NULL);
}

MclStatus MclEventService_On5sTimeout() {
	MclList *ids = MclList_CreateDefault();
	MCL_ASSERT_VALID_PTR(ids);

	MCL_DEFER_ENABLE_WITH(MclEventService_ReleaseList, ids);

	MCL_ASSERT_SUCC_CALL(MclEntityRepo_AcceptConst(MclEventService_CollectEntityIds, ids));
	MCL_ASSERT_SUCC_CALL(MclList_Accept(ids, MclEventService_ClearEntity, NULL));
	MCL_LOG_SUCC("Event Service: Clear entities on 5S timer OK!");
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclEventService_DoubleAggregator(MclAggregator *aggregator, void *arg) {
	MCL_ASSERT_SUCC_CALL(MclAggregator_DoubleEntities(aggregator));
	MCL_LOG_DBG("Double aggregator (%u).", MclAggregator_GetId(aggregator));
	return MCL_SUCCESS;
}

MclStatus MclEventService_OnDoubleIsr() {
	MCL_ASSERT_SUCC_CALL(MclAggregatorRepo_Accept(MclEventService_DoubleAggregator, NULL));
	MCL_LOG_SUCC("Event Service: Aggregators doubled on ISR OK!");
	return MCL_SUCCESS;
}
