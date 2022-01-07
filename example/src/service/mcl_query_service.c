#include "mcl/service/mcl_query_service.h"
#include "repo/mcl_entity_repo.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

MclInteger MclQueryService_QueryEntityValue(MclEntityId entityId) {
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(entityId));

	MCL_LOCK_OBJ_AUTO const MclEntity *entity = MclEntityRepo_FetchConst(entityId);
	MCL_ASSERT_VALID_PTR_NIL(entity);

	MclInteger result = MclEntity_GetValue(entity);

	MCL_LOG_SUCC("Query Service: Query value (%u) of entity (%u) OK!", entityId, result);
	return result;
}
