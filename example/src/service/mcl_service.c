#include "mcl/service/mcl_service.h"
#include "mcl/log/log.h"

MclStatus MclService_Init() {
	MCL_LOG_DBG("Mcl Service Init.");
	return MCL_SUCCESS;
}

void MclService_Destroy() {
	MCL_LOG_DBG("Mcl Service Destroy.");
}
