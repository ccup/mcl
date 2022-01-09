#include "mcl/service/mcl_config_service.h"
#include "mcl/service/mcl_control_service.h"
#include "mcl/service/mcl_event_service.h"
#include "mcl/service/mcl_query_service.h"
#include "mcl/thread/thread.h"
#include <unistd.h>

const static size_t ENTITY_COUNT = 16;
const static size_t AGGREGATOR_COUNT = 4;
const static size_t TIME_COUNT = ENTITY_COUNT * 2 + AGGREGATOR_COUNT;

//#define SCHEDULE_DELAY(N) sleep(N)
#define SCHEDULE_DELAY(N) MclThread_Yield()

MCL_PRIVATE void* RunAggregatorConfigService(void *data) {
	MCL_LOG_INFO("MCL Example Aggregator Config Service Start...");

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclConfigService_CreateAggregator(aggregatorId);
		SCHEDULE_DELAY(4);
	}

	SCHEDULE_DELAY(1);

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclConfigService_DeleteAggregator(aggregatorId);
		SCHEDULE_DELAY(4);
	}

	MCL_LOG_INFO("MCL Example Aggregator Config Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunEntityConfigService(void *data) {
	MCL_LOG_INFO("MCL Example Entity Config Service Start...");

	for (MclEntityId aggregatorId = 0; aggregatorId< ENTITY_COUNT; aggregatorId++) {
		MclConfigService_CreateEntity(aggregatorId);
		SCHEDULE_DELAY(1);
	}

	SCHEDULE_DELAY(1);

	for (MclEntityId aggregatorId = 0; aggregatorId< ENTITY_COUNT; aggregatorId++) {
		MclConfigService_DeleteEntity(aggregatorId);
		SCHEDULE_DELAY(1);
	}

	MCL_LOG_INFO("MCL Example Entity Config Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunRelationControlService(void *data) {
	MCL_LOG_INFO("MCL Example Releation Control Service Start...");

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclEntityId entityId = aggregatorId * 4;
		MclControlService_AddEntityToAggregator(entityId, aggregatorId);
		SCHEDULE_DELAY(1);
	}

	SCHEDULE_DELAY(1);

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclEntityId entityId = aggregatorId * 4;
		MclControlService_RemoveEntityFromAggregator(entityId, aggregatorId);
		SCHEDULE_DELAY(1);
	}

	MCL_LOG_INFO("MCL Example Releation Control Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunValueControlService(void *data) {
	MCL_LOG_INFO("MCL Example Value Control Service Start...");

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclEntityId entityId = aggregatorId * 4;
		MclControlService_DoubleEntitesInAggregator(aggregatorId);
		MclControlService_DoubleEntity(entityId);
		SCHEDULE_DELAY(1);
	}

	MCL_LOG_INFO("MCL Example Value Control Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunTimerService(void *data) {
	MCL_LOG_INFO("MCL Example Timer Service Start...");

	for (uint32_t i = 0; i < TIME_COUNT; i++) {
		MclEventService_On1sTimeout();
		if ((i + 1) % 5 == 0) {
			MclEventService_On5sTimeout();
		}
		SCHEDULE_DELAY(1);
	}

	MCL_LOG_INFO("MCL Example Timer Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunIsrService(void *data) {
	MCL_LOG_INFO("MCL Example ISR Service Start...");

	for (uint32_t i = 0; i < TIME_COUNT; i++) {
		MclEventService_OnDoubleIsr();
		SCHEDULE_DELAY(1);
	}

	MCL_LOG_INFO("MCL Example ISR Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunStatusQueryService(void *data) {
	MCL_LOG_INFO("MCL Example Status Query Service Start...");

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclQueryService_QueryEntityCountIn(aggregatorId);
		SCHEDULE_DELAY(8);
	}

	MCL_LOG_INFO("MCL Example Status Query Service Exit!");
	return NULL;
}

MCL_PRIVATE void* RunValueQueryService(void *data) {
	MCL_LOG_INFO("MCL Example Value Query Service Start...");

	for (MclAggregatorId aggregatorId = 0; aggregatorId< AGGREGATOR_COUNT; aggregatorId++) {
		MclEntityId entityId = aggregatorId * 4;
		MclQueryService_QuerySumValueOf(aggregatorId);
		MclQueryService_QueryValueOf(entityId);
		SCHEDULE_DELAY(2);
	}

	MCL_LOG_INFO("MCL Example Value Query Service Exit!");
	return NULL;
}

MCL_PRIVATE MclThread StartThread(void*(*run)(void*)) {
	MclThread thread;
	MclThread_Create(&thread, NULL, run, NULL);
	return thread;
}

int main() {
	MCL_LOG_INFO("MCL Example Start...");

	MclThread aggregatorConfigThread = StartThread(RunAggregatorConfigService);
	MclThread entityConfigThread     = StartThread(RunEntityConfigService);
	MclThread releationControlThread = StartThread(RunRelationControlService);
	MclThread valueControlThread     = StartThread(RunValueControlService);
	MclThread timerThread            = StartThread(RunTimerService);
	MclThread isrThread              = StartThread(RunIsrService);
	MclThread statusQueryThread      = StartThread(RunStatusQueryService);
	MclThread valueQueryThread       = StartThread(RunValueQueryService);

	MclThread_Join(aggregatorConfigThread, NULL);
	MclThread_Join(entityConfigThread,     NULL);
	MclThread_Join(releationControlThread, NULL);
	MclThread_Join(valueControlThread,     NULL);
	MclThread_Join(timerThread,       NULL);
	MclThread_Join(isrThread,         NULL);
	MclThread_Join(statusQueryThread, NULL);
	MclThread_Join(valueQueryThread,  NULL);

	MCL_LOG_INFO("Mcl Example Exit!");
	return MCL_SUCCESS;
}

