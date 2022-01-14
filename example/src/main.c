#include "mcl/service/mcl_config_service.h"
#include "mcl/service/mcl_control_service.h"
#include "mcl/service/mcl_event_service.h"
#include "mcl/service/mcl_query_service.h"
#include "mcl/service/statistics/mcl_statistics.h"
#include "mcl/thread/thread_launcher.h"
#include "mcl/array/array_size.h"
#include "mcl/time/time.h"
#include "mcl/algo/loop.h"
#include <stdlib.h>

typedef struct {
	MclSize aggregatorCount;
	MclSize entityCount;
	MclTimeSecDiff aggregatorIntervalSec;
	MclTimeSecDiff entityIntervalSec;
	MclTimeSecDiff isrIntervalSec;
	MclTimeSecDiff intervalSec;
	MclTimeSecDiff totalTimeSec;
} ExampleConfig;

MCL_PRIVATE void ExampleThread_Delay(MclTimeSecDiff sec) {
	sec ? MclTimeSec_Delay(sec) : MclThread_Yield();
}

MCL_PRIVATE void ExampleThread_ConfigAggregator(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_SIZE(aggregatorId, cfg->aggregatorCount) {
		MclConfigService_CreateAggregator(aggregatorId);
		ExampleThread_Delay(cfg->aggregatorIntervalSec);
	}
	ExampleThread_Delay(cfg->intervalSec);
	MCL_LOOP_FOREACH_SIZE(aggregatorId, cfg->aggregatorCount) {
		MclConfigService_DeleteAggregator(aggregatorId);
		ExampleThread_Delay(cfg->aggregatorIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ConfigEntity(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclConfigService_CreateEntity(entityId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
	ExampleThread_Delay(cfg->intervalSec);
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclConfigService_DeleteEntity(entityId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ControlRelation(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclAggregatorId aggregatorId = entityId % cfg->aggregatorCount;
		MclControlService_AddEntityToAggregator(entityId, aggregatorId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
	ExampleThread_Delay(cfg->intervalSec);
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclAggregatorId aggregatorId = entityId % cfg->aggregatorCount;
		MclControlService_RemoveEntityFromAggregator(entityId, aggregatorId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_ControlValue(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclControlService_UpdateEntityValue(entityId, (MclInteger)(rand() % 10));
		MclControlService_DoubleEntityValue(entityId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
	ExampleThread_Delay(cfg->intervalSec);
	MCL_LOOP_FOREACH_SIZE(aggregatorId, cfg->aggregatorCount) {
		MclControlService_DoubleEntitesInAggregator(aggregatorId);
		ExampleThread_Delay(cfg->aggregatorIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_EventOnTimer(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_STEP(i, cfg->totalTimeSec, cfg->intervalSec) {
		MclEventService_On1sTimeout();
		if ((i + 1) % 5 == 0) {
			MclEventService_On5sTimeout();
		}
		ExampleThread_Delay(cfg->intervalSec);
	}
}

MCL_PRIVATE void ExampleThread_EventOnIsr(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_STEP(i, cfg->totalTimeSec, cfg->isrIntervalSec) {
		MclEventService_OnDoubleIsr();
		ExampleThread_Delay(cfg->isrIntervalSec);
	}
}

MCL_PRIVATE void ExampleThread_QueryCount(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_STEP(i, cfg->totalTimeSec, cfg->intervalSec) {
		MclQueryService_QueryEntityCount();
		MclQueryService_QueryAggregatorCount();
		ExampleThread_Delay(cfg->intervalSec);
	}
}

MCL_PRIVATE void ExampleThread_QueryValue(void *ctxt) {
	ExampleConfig *cfg = (ExampleConfig*)ctxt;
	MCL_LOOP_FOREACH_SIZE(entityId, cfg->entityCount) {
		MclQueryService_QueryValueOf(entityId);
		ExampleThread_Delay(cfg->entityIntervalSec);
	}
	ExampleThread_Delay(cfg->intervalSec);
	MCL_LOOP_FOREACH_SIZE(aggregatorId, cfg->aggregatorCount) {
		MclQueryService_QueryEntityCountOfAggregator(aggregatorId);
		MclQueryService_QuerySumValueOf(aggregatorId);
		ExampleThread_Delay(cfg->aggregatorIntervalSec);
	}
}

MCL_PRIVATE ExampleConfig fastConfig = {
		.aggregatorCount = 4,
		.entityCount = 16,
		.aggregatorIntervalSec = 0,
		.entityIntervalSec = 0,
		.isrIntervalSec = 0,
		.intervalSec = 0,
		.totalTimeSec = 0,
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

#define EXAMPLE_THREAD(NAME, EXECUTE) MCL_THREAD_INFO(NAME, EXECUTE, NULL, &normalConfig)

MCL_PRIVATE MclThreadInfo exampleThreads[] = {
		EXAMPLE_THREAD("AggCfg" , ExampleThread_ConfigAggregator),
		EXAMPLE_THREAD("EntyCfg", ExampleThread_ConfigEntity),
		EXAMPLE_THREAD("RelCtrl", ExampleThread_ControlRelation),
		EXAMPLE_THREAD("ValCtrl", ExampleThread_ControlValue),
		EXAMPLE_THREAD("TimeEnv", ExampleThread_EventOnTimer),
		EXAMPLE_THREAD("IsrEnv" , ExampleThread_EventOnIsr),
		EXAMPLE_THREAD("CntQry",  ExampleThread_QueryCount),
		EXAMPLE_THREAD("ValQry" , ExampleThread_QueryValue),
};

MclStatus MclExample_StatisticsResult() {
	MclStatistics statistics = MclQueryService_QueryStatistics();
	MCL_LOG("--------------------------------------------------\n");
	MCL_ASSERT_TRUE(statistics.unreleasedAggregatorCount == 0);
	MCL_ASSERT_TRUE(statistics.unreleasedEntityCount == 0);
	MCL_LOG_SUCC("Mcl Example OK!");
	return MCL_SUCCESS;
}

int main() {
	MCL_ASSERT_SUCC_CALL(MclThreadLauncher_Launch(exampleThreads, MCL_ARRAY_SIZE(exampleThreads)));
	MclThreadLauncher_WaitDone(exampleThreads, MCL_ARRAY_SIZE(exampleThreads));
	return MclExample_StatisticsResult();
}

