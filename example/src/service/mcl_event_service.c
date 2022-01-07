#include "mcl/service/mcl_event_service.h"
#include "repo/mcl_entity_repo.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

typedef struct {
	uint32_t count;
} EntityCounter;

MCL_PRIVATE MclStatus MclEventService_CountEntity(const MclEntity *entity, void *arg) {
	EntityCounter *count = (EntityCounter*)arg;
	MCL_LOG_DBG("Count entity (%u : %u)", MclEntity_GetId(entity), MclEntity_GetValue(entity));
	(*count)++;
	return MCL_SUCCESS;
}

MclStatus MclEventService_On1sTimeout() {
	EntityCounter counter = {0};
	MCL_ASSERT_SUCC_CALL(MclEntityRepo_AcceptConst(MclEventService_CountEntity, &counter));
	MCL_LOG_SUCC("Event Service: entities count (%u) on 1S timer OK", counter.count);
	return MCL_SUCCESS;
}


MCL_PRIVATE MclStatus MclEventService_DoubleEntity(MclEntity *entity, void *arg) {
	MCL_ASSERT_VALID_PTR(MclEntity_DoubleValue(entity));
	return MCL_SUCCESS;
}

MclStatus MclEventService_OnDoubleIsr() {
	MCL_ASSERT_SUCC_CALL(MclEntityRepo_Accept(MclEventService_DoubleEntity, NULL));
	MCL_LOG_SUCC("Event Service: entities doubled on ISR OK!");
	return MCL_SUCCESS;
}
