#include "mcl/service/mcl_control_service.h"
#include "mcl/domain/value/mcl_status.h"
#include "repo/mcl_entity_repo.h"
#include "entity/mcl_entity.h"
#include "repo/mcl_aggregator_repo.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MclStatus MclControlService_AddEntityToAggregator(MclEntityId entityId, MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));
	MCL_ASSERT_TRUE(MclEntityId_IsValid(aggregatorId));

	MCL_LOCK_OBJ_AUTO MclAggregator *aggregator = MclAggregatorRepo_Fetch(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Control Service: not found aggregator (%u)!", aggregatorId);
		return MCL_STATUS_AGGREGATOR_NOT_FOUND;
	}

	MCL_LOCK_OBJ_AUTO MclEntity *entity = MclEntityRepo_Fetch(entityId);
	if (!entity) {
		MCL_LOG_WARN("Control Service: not found entity (%u)!", entityId);
		return MCL_STATUS_ENTITY_NOT_FOUND;
	}

	MclStatus status = MclAggregator_AddEntity(aggregator, entity);
	MCL_ASSERT_TRUE(!MCL_FAILED(status));

	if (MclStatus_IsNothingChanged(status)) {
		MCL_LOG_WARN("Control Service: Add entity (%u) to aggregagor (%u) but already in!", entityId, aggregatorId);
	} else {
		MCL_LOG_SUCC("Control Service: Add entity (%u) to aggregagor (%u) OK!", entityId, aggregatorId);
	}
	return MCL_SUCCESS;
}

MclStatus MclControlService_RemoveEntityFromAggregator(MclEntityId entityId, MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));
	MCL_ASSERT_TRUE(MclEntityId_IsValid(aggregatorId));

	MCL_LOCK_OBJ_AUTO MclAggregator *aggregator = MclAggregatorRepo_Fetch(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Control Service: not found aggregator (%u)!", aggregatorId);
		return MCL_STATUS_AGGREGATOR_NOT_FOUND;
	}

	MclStatus status = MclAggregator_RemoveEntity(aggregator, entityId);
	MCL_ASSERT_TRUE(!MCL_FAILED(status));

	if (MclStatus_IsNothingChanged(status)) {
		MCL_LOG_WARN("Control Service: Remove entity (%u) from aggregagor (%u) but not found!", entityId, aggregatorId);
	} else {
		MCL_LOG_SUCC("Control Service: Remove entity (%u) from aggregagor (%u) OK!", entityId, aggregatorId);
	}
	return MCL_SUCCESS;
}

MclStatus MclControlService_DoubleEntitesInAggregator(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(aggregatorId));

	MCL_LOCK_OBJ_AUTO MclAggregator *aggregator = MclAggregatorRepo_Fetch(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Control Service: not found aggregator (%u)!", aggregatorId);
		return MCL_STATUS_AGGREGATOR_NOT_FOUND;
	}

	MclStatus status = MclAggregator_DoubleEntities(aggregator);
	MCL_ASSERT_TRUE(!MCL_FAILED(status));

	if (MclStatus_IsNothingChanged(status)) {
		MCL_LOG_SUCC("Control Service: Double all entities in aggregagor (%u) but nothing changed!", aggregatorId);
	} else {
		MCL_LOG_SUCC("Control Service: Double all entities in aggregagor (%u) OK!", aggregatorId);
	}
	return MCL_SUCCESS;
}

MclStatus MclControlService_UpdateEntityValue(MclEntityId entityId, MclInteger value) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));
	MCL_ASSERT_TRUE(MclInteger_IsValid(value));

	MCL_LOCK_OBJ_AUTO MclEntity *entity = MclEntityRepo_Fetch(entityId);
	if (!entity) {
		MCL_LOG_WARN("Control Service: not found entity (%u)!", entityId);
		return MCL_STATUS_ENTITY_NOT_FOUND;
	}

	MclStatus status = MclEntity_UpdateValue(entity, value);
	MCL_ASSERT_TRUE(!MCL_FAILED(status));

	if (MclStatus_IsNothingChanged(status)) {
		MCL_LOG_WARN("Control Service: Update entity (%u) value (%u) but nothing changed!", entityId, value);
	} else {
		MCL_LOG_SUCC("Control Service: update entity (%u) value to (%u) OK!", entityId, value);
	}
	return MCL_SUCCESS;
}

MclStatus MclControlService_DoubleEntityValue(MclEntityId entityId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	MCL_LOCK_OBJ_AUTO MclEntity *entity = MclEntityRepo_Fetch(entityId);
	if (!entity) {
		MCL_LOG_WARN("Control Service: not found entity (%u)!", entityId);
		return MCL_STATUS_ENTITY_NOT_FOUND;
	}

	MclStatus status = MclEntity_DoubleValue(entity);
	MCL_ASSERT_TRUE(!MCL_FAILED(status));

	if (MclStatus_IsNothingChanged(status)) {
		MCL_LOG_WARN("Control Service: Double entity (%u) but nothing changed!", entityId);
	} else {
		MCL_LOG_SUCC("Control Service: Double entity (%u) OK!", entityId);
	}
	return MCL_SUCCESS;
}
