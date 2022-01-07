#include "mcl/service/mcl_control_service.h"
#include "repo/mcl_entity_repo.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MclStatus MclConfigService_DoubleEntity(MclEntityId entityId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	MCL_LOCK_OBJ_AUTO MclEntity *entity = MclEntityRepo_Fetch(entityId);
	MCL_ASSERT_VALID_PTR(entity);

	MCL_ASSERT_SUCC_CALL(MclEntity_DoubleValue(entity));

	MCL_LOG_SUCC("Control Service: Double entity (%u) OK!", entityId);
	return MCL_SUCCESS;
}
