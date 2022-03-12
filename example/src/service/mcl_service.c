#include "mcl/service/mcl_service.h"
#include "mcl/infra/allocator/mcl_allocator_type.h"
#include "mcl/log/log.h"
#include "factory/mcl_entity_factory.h"

MclStatus MclService_Init() {
	MCL_LOG_DBG("Mcl Service Init.");
	MclEntityFactory_Init(MCL_ALLOCATOR_LOCK_OBJ);
	return MCL_SUCCESS;
}

void MclService_Destroy() {
	MCL_LOG_DBG("Mcl Service Destroy.");
}
