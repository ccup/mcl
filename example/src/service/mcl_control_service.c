#include "mcl/service/mcl_control_service.h"
#include "repo/mcl_entity_repo.h"
#include "entity/mcl_entity.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MclStatus MclConfigService_DoubleEntity(MclEntityId entityId) {
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	MCL_LOCK_OBJ_AUTO MclEntity *entity = MclEntityRepo_Fetch(entityId);
	MCL_ASSERT_VALID_PTR(entity);

	MclStatus ret = MclEntity_DoubleValue(entity);
	MCL_ASSERT_TRUE(!MCL_FAILED(ret));

	if (MclStatus_IsNothingChanged(ret)) {
		MCL_LOG_SUCC("Control Service: Entity (%u) not changed!", entityId);
	} else {
		MCL_LOG_SUCC("Control Service: Double entity (%u) OK!", entityId);
	}
	return MCL_SUCCESS;
}
