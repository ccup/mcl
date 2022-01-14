#include "mcl/service/mcl_event_service.h"
#include "repo/mcl_entity_repo.h"
#include "entity/mcl_entity.h"
#include "repo/mcl_aggregator_repo.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

typedef struct {
	MclSize count;
} EntityCounter;

MCL_PRIVATE MclStatus MclEventService_CountEntity(const MclEntity *entity, void *arg) {
	EntityCounter *counter = (EntityCounter*)arg;
	MCL_LOG_DBG("Count entity (%u : %u).", MclEntity_GetId(entity), MclEntity_GetValue(entity));
	counter->count++;
	return MCL_SUCCESS;
}

MclStatus MclEventService_On1sTimeout() {
	EntityCounter counter = {0};
	MCL_ASSERT_SUCC_CALL(MclEntityRepo_AcceptConst(MclEventService_CountEntity, &counter));
	MCL_LOG_SUCC("Event Service: Count (%u) entities on 1S timer OK!", counter.count);
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclEventService_ClearEntity(MclEntity *entity, void *arg) {
	MclEntity_ClearValue(entity);
	MCL_LOG_DBG("Clear entity (%u).", MclEntity_GetId(entity));
	return MCL_SUCCESS;
}

MclStatus MclEventService_On5sTimeout() {
	MCL_ASSERT_SUCC_CALL(MclEntityRepo_Accept(MclEventService_ClearEntity, NULL));
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
