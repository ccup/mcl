#include "mcl/service/mcl_config_service.h"
#include "factory/mcl_entity_factory.h"
#include "factory/mcl_aggregator_factory.h"
#include "repo/mcl_entity_repo.h"
#include "repo/mcl_aggregator_repo.h"
#include "mcl/assert.h"

MclStatus MclConfigService_CreateEntity(MclEntityId entityId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	MclEntity *entity = MclEntityFactory_CreateLockObj(entityId);
	MCL_ASSERT_VALID_PTR(entity);

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
	MCL_ASSERT_VALID_PTR(entity);

	MclEntityFactory_DeleteLockObj(entity);
	MCL_LOG_SUCC("Config Service: Delete entity (%u) OK!", entityId);
	return MCL_SUCCESS;
}

MclStatus MclConfigService_CreateAggregator(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE(MclAggregatorId_IsValid(aggregatorId));

	MclAggregator *aggregator = MclAggregatorFactory_CreateLockObj(aggregatorId);
	MCL_ASSERT_VALID_PTR(aggregator);

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
	MCL_ASSERT_VALID_PTR(aggregator);

	MclAggregatorFactory_DeleteLockObj(aggregator);
	MCL_LOG_SUCC("Config Service: Delete aggregator (%u) OK!", aggregatorId);
	return MCL_SUCCESS;
}
