#include "mcl/service/mcl_config_service.h"
#include "mcl/service/mcl_control_service.h"
#include "mcl/service/mcl_event_service.h"
#include "mcl/service/mcl_query_service.h"
#include "mcl/array/array_size.h"
#include "mcl/thread/thread.h"
#include "mcl/time/time.h"
#include "mcl/algo/loop.h"

typedef struct {
	size_t aggregatorCount;
	size_t entityCount;
	MclTimeSecDiff aggregatorIntervalSec;
	MclTimeSecDiff entityIntervalSec;
	MclTimeSecDiff isrIntervalSec;
	MclTimeSecDiff intervalSec;
	MclTimeSecDiff totalTimeSec;
} ExampleConfig;

MCL_TYPE(ExampleThread) {
	const char *name;
	MclThread thread;
	void (*execute)(ExampleThread*);
	ExampleConfig *config;
};

MCL_PRIVATE void Example_Thread_WaitDone(ExampleThread *self) {
	MCL_ASSERT_SUCC_CALL_VOID(MclThread_Join(self->thread, NULL));
}

MCL_PRIVATE void* ExampleThread_Execute(void *data) {
	ExampleThread *thread = (ExampleThread*)data;

	MCL_LOG_INFO("Thread %s enter!", thread->name);
	MCL_ASSERT_SUCC_CALL_NIL(MclThread_SetName(thread->thread, thread->name));
	thread->execute(thread);
	MCL_LOG_INFO("Thread %s exit!", thread->name);
	return NULL;
}

MCL_PRIVATE MclStatus ExampleThread_Launch(ExampleThread* thread) {
	MCL_ASSERT_SUCC_CALL(MclThread_Create(&thread->thread, NULL, ExampleThread_Execute, thread));
	return MCL_SUCCESS;
}

MCL_PRIVATE void ExampleThread_Delay(MclTimeSecDiff sec) {
	MclTimeSec_Delay(sec);
}

MCL_PRIVATE void ExampleThread_ConfigAggregator(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclConfigService_CreateAggregator(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclConfigService_DeleteAggregator(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ConfigEntity(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclConfigService_CreateEntity(entityId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclConfigService_DeleteEntity(entityId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ControlRelation(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclAggregatorId aggregatorId = entityId % thread->config->aggregatorCount;
		MclControlService_AddEntityToAggregator(entityId, aggregatorId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclAggregatorId aggregatorId = entityId % thread->config->aggregatorCount;
		MclControlService_RemoveEntityFromAggregator(entityId, aggregatorId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ControlValue(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclControlService_DoubleEntity(entityId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclControlService_DoubleEntitesInAggregator(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_EventOnTimer(ExampleThread *thread) {
	MCL_LOOP_FOREACH_STEP(i, thread->config->totalTimeSec, thread->config->intervalSec) {
		MclEventService_On1sTimeout();
		if ((i + 1) % 5 == 0) {
			MclEventService_On5sTimeout();
		}
		ExampleThread_Delay(thread->config->intervalSec);
	}
}

MCL_PRIVATE void ExampleThread_EventOnIsr(ExampleThread *thread) {
	MCL_LOOP_FOREACH_STEP(i, thread->config->totalTimeSec, thread->config->isrIntervalSec) {
		MclEventService_OnDoubleIsr();
		ExampleThread_Delay(thread->config->isrIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_QueryStatus(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclQueryService_QueryEntityCountIn(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclQueryService_QueryEntityCountIn(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_QueryValue(ExampleThread *thread) {
	MCL_LOOP_FOREACH_INDEX(entityId, thread->config->entityCount) {
		MclQueryService_QueryValueOf(entityId);
		ExampleThread_Delay(thread->config->entityIntervalSec);
	}
	ExampleThread_Delay(thread->config->intervalSec);
	MCL_LOOP_FOREACH_INDEX(aggregatorId, thread->config->aggregatorCount) {
		MclQueryService_QuerySumValueOf(aggregatorId);
		ExampleThread_Delay(thread->config->aggregatorIntervalSec);
	}
}

MCL_PRIVATE ExampleConfig fastConfig = {
		.aggregatorCount = 4,
		.entityCount = 16,
		.aggregatorIntervalSec = MCL_TIME_SEC_DIFF_INVALID,
		.entityIntervalSec = MCL_TIME_SEC_DIFF_INVALID,
		.isrIntervalSec = MCL_TIME_SEC_DIFF_INVALID,
		.intervalSec = MCL_TIME_SEC_DIFF_INVALID,
		.totalTimeSec = MCL_TIME_SEC_DIFF_INVALID,
};

MCL_PRIVATE ExampleConfig normalConfig = {
		.aggregatorCount = 4,
		.entityCount = 16,
		.aggregatorIntervalSec = 4,
		.entityIntervalSec = 1,
		.isrIntervalSec = 2,
		.intervalSec = 1,
		.totalTimeSec = 32,
};

#define EXAMPLE_THREAD(NAME, EXECUTE) {.name = NAME, .execute = EXECUTE, .config = &normalConfig}

MCL_PRIVATE ExampleThread exampleThreads[] = {
		EXAMPLE_THREAD("AggCfg" , ExampleThread_ConfigAggregator),
		EXAMPLE_THREAD("EntyCfg", ExampleThread_ConfigEntity),
		EXAMPLE_THREAD("RelCtrl", ExampleThread_ControlRelation),
		EXAMPLE_THREAD("ValCtrl", ExampleThread_ControlValue),
		EXAMPLE_THREAD("TimeEnv", ExampleThread_EventOnTimer),
		EXAMPLE_THREAD("IsrEnv" , ExampleThread_EventOnIsr),
		EXAMPLE_THREAD("StatQry", ExampleThread_QueryStatus),
		EXAMPLE_THREAD("ValQry" , ExampleThread_QueryValue),
};

MCL_PRIVATE MclStatus ExampleThreads_Launch() {
	MCL_LOG_INFO("Mcl Example threads launch...");
	MCL_LOOP_FOREACH_INDEX(i, MCL_ARRAY_SIZE(exampleThreads)) {
		MCL_ASSERT_SUCC_CALL(ExampleThread_Launch(&exampleThreads[i]));
	}
	return MCL_SUCCESS;
}

MCL_PRIVATE void ExampleThreads_WaitDone() {
	MCL_LOOP_FOREACH_INDEX(i, MCL_ARRAY_SIZE(exampleThreads)) {
		Example_Thread_WaitDone(&exampleThreads[i]);
	}
	MCL_LOG_SUCC("Mcl Example threads done!!!");
}

#include <stdlib.h>

int main() {
	MCL_ASSERT_SUCC_CALL(ExampleThreads_Launch());
	ExampleThreads_WaitDone();
	return 0;
}

