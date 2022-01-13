#include "mcl/service/mcl_config_service.h"
#include "mcl/domain/value/mcl_status.h"
#include "factory/mcl_entity_factory.h"
#include "factory/mcl_aggregator_factory.h"
#include "repo/mcl_entity_repo.h"
#include "repo/mcl_aggregator_repo.h"
#include "aggregator/mcl_aggregator.h"
#include "entity/mcl_entity.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MclStatus MclConfigService_CreateEntity(MclEntityId entityId, const MclEntityConfig *config) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));
	MCL_ASSERT_VALID_PTR(config);

	MclEntity *entity = MclEntityFactory_CreateLockObj(entityId, config);
	if (!entity) {
		MCL_LOG_WARN("Config Service: not found entity (%u)!", entityId);
		return MCL_STATUS_ENTITY_NOT_FOUND;
	}

	if (MCL_FAILED(MclEntityRepo_Insert(entity))) {
		MclEntityFactory_DeleteLockObj(entity);
		MCL_LOG_ERR("Config Service: Insert entity (%u) to repo failed!", entityId);
		return MCL_FAILURE;
	}
	MCL_LOG_SUCC("Config Service: Create entity (%u) OK!", entityId);
	return MCL_SUCCESS;
}

MclStatus MclConfigService_DeleteEntity(MclEntityId entityId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	MclEntity *entity = MclEntityRepo_Remove(entityId);
	if (!entity) {
		MCL_LOG_WARN("Config Service: not found entity (%u)!", entityId);
		return MCL_STATUS_ENTITY_NOT_FOUND;
	}

	MclAggregatorId aggregatorId = MclEntity_GetAggregatorId(entity);
	if (MclAggregatorId_IsValid(aggregatorId)) {
		MCL_LOCK_OBJ_AUTO MclAggregator *aggregator = MclAggregatorRepo_Fetch(aggregatorId);
		if (aggregator) {
			(void)MclAggregator_RemoveEntity(aggregator, entityId);
		}
	}

	MclEntityFactory_DeleteLockObj(entity);
	MCL_LOG_SUCC("Config Service: Delete entity (%u) OK!", entityId);
	return MCL_SUCCESS;
}

MclStatus MclConfigService_CreateAggregator(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclAggregatorId_IsValid(aggregatorId));

	MclAggregator *aggregator = MclAggregatorFactory_CreateLockObj(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Config Service: not found aggregator (%u)!", aggregatorId);
		return MCL_STATUS_AGGREGATOR_NOT_FOUND;
	}

	if (MCL_FAILED(MclAggregatorRepo_Insert(aggregator))) {
		MclAggregatorFactory_DeleteLockObj(aggregator);
		MCL_LOG_ERR("Config Service: Insert aggregator (%u) to repo failed!", aggregatorId);
		return MCL_FAILURE;
	}
	MCL_LOG_SUCC("Config Service: Create aggregator (%u) OK!", aggregatorId);
	return MCL_SUCCESS;
}

MclStatus MclConfigService_DeleteAggregator(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclAggregatorId_IsValid(aggregatorId));

	MclAggregator *aggregator = MclAggregatorRepo_Remove(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Config Service: not found aggregator (%u)!", aggregatorId);
		return MCL_STATUS_AGGREGATOR_NOT_FOUND;
	}

	MclAggregatorFactory_DeleteLockObj(aggregator);
	MCL_LOG_SUCC("Config Service: Delete aggregator (%u) OK!", aggregatorId);
	return MCL_SUCCESS;
}
