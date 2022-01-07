#include "factory/mcl_entity_factory.h"
#include "entity/private/mcl_entity_private.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/lock/lockobj.h"
#include "mcl/lock/lockptr.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclObjectFactory_EntityDestructor(void *obj, void *arg) {
	MclEntity_Destroy((MclEntity*)obj);
}

MCL_PRIVATE void MclObjectFactory_EntityDeleter(void *obj, void *arg) {
	MclEntityFactory_Delete((MclEntity*)obj);
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_Create(MclEntityId id) {
	MclEntity *self = MCL_MALLOC(sizeof(MclEntity));
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id))) {
		MCL_LOG_ERR("Initialize object (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}
	return self;
}

void MclEntityFactory_Delete(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclEntity_Destroy(self);
	MCL_FREE(self);
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_CreateSharedPtr(MclEntityId id) {
	MclEntity *self = MCL_SHARED_PTR(MclEntity, {id, 0}, MclObjectFactory_EntityDestructor, NULL);
	MCL_ASSERT_VALID_PTR_NIL(self);
	return self;
}

void MclEntityFactory_DeleteSharedPtr(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclSharedPtr_Delete(self);
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_CreateLockObj(MclEntityId id) {
	MCL_LOCK_OBJ_CREATE(MclEntity, self);
	MCL_ASSERT_VALID_PTR(self);

	if (MCL_FAILED(MclEntity_Init(self, id))) {
		MCL_LOG_ERR("Initialize lock object (%u) failed!", id);
		MclLockObj_Delete(self, NULL, NULL);
		return NULL;
	}
	return self;
}

void MclEntityFactory_DeleteLockObj(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclLockObj_Delete(self, MclObjectFactory_EntityDestructor, NULL);
}

///////////////////////////////////////////////////////////
MclLockPtr* MclEntityFactory_CreateLockPtr(MclEntityId id) {
	MclEntity *self = MclEntityFactory_Create(id);
	MCL_ASSERT_VALID_PTR_NIL(self);

	return MclLockPtr_Create(self);
}

void MclEntityFactory_DeleteLockPtr(MclLockPtr *ptr) {
	MCL_ASSERT_VALID_PTR_VOID(ptr);

	MclLockPtr_Delete(ptr, MclObjectFactory_EntityDeleter, NULL);
}
