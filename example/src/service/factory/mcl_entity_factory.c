#include "factory/mcl_entity_factory.h"
#include "entity/mcl_entity.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/lock/lockobj.h"
#include "mcl/lock/lockptr.h"
#include "mcl/lock/atom.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE void MclEntityFactory_DestroyEntity(void *obj, void *arg) {
	MclEntity_Destroy((MclEntity*)obj);
}

MCL_PRIVATE void MclEntityFactory_DeleteEntity(void *obj, void *arg) {
	MclEntityFactory_Delete((MclEntity*)obj);
}

///////////////////////////////////////////////////////////
MclAtom entityCount = 0;

size_t MclEntityFactory_GetUnreleasedCount() {
	return entityCount;
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_Create(MclEntityId id) {
	MclEntity *self = MCL_MALLOC(MCL_ENTITY_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id))) {
		MCL_LOG_ERR("Initialize object (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}

	MclAtom_Add(&entityCount, 1);
	return self;
}

void MclEntityFactory_Delete(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclEntity_Destroy(self);
	MCL_FREE(self);

	MclAtom_Sub(&entityCount, 1);
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_CreateSharedPtr(MclEntityId id) {
	MclEntity *self = MclSharedPtr_Create(MCL_ENTITY_SIZE, MclEntityFactory_DestroyEntity, NULL);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id))) {
		MCL_LOG_ERR("Initialize shared ptr (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}

	MclAtom_Add(&entityCount, 1);
	return self;
}

void MclEntityFactory_DeleteSharedPtr(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclSharedPtr_Delete(self);
	MclAtom_Sub(&entityCount, 1);
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_CreateLockObj(MclEntityId id) {
	MclEntity *self = (MclEntity*)MclLockObj_Create(MCL_ENTITY_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id))) {
		MCL_LOG_ERR("Initialize lock object (%u) failed!", id);
		MclLockObj_Delete(self, NULL, NULL);
		return NULL;
	}
	MclAtom_Add(&entityCount, 1);
	return self;
}

void MclEntityFactory_DeleteLockObj(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLockObj_Delete(self, MclEntityFactory_DestroyEntity, NULL);
	MclAtom_Sub(&entityCount, 1);
}

///////////////////////////////////////////////////////////
MclLockPtr* MclEntityFactory_CreateLockPtr(MclEntityId id) {
	MclEntity *self = MclEntityFactory_Create(id);
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclLockPtr *ptr = MclLockPtr_Create(self);
	if (!ptr) {
		MCL_LOG_ERR("Initialize lock ptr (%u) failed!", id);
		MclLockPtr_Delete(ptr, MclEntityFactory_DeleteEntity, NULL);
		return NULL;
	}
	MclAtom_Add(&entityCount, 1);
	return ptr;
}

void MclEntityFactory_DeleteLockPtr(MclLockPtr *ptr) {
	MCL_ASSERT_VALID_PTR_VOID(ptr);

	MclLockPtr_Delete(ptr, MclEntityFactory_DeleteEntity, NULL);
	MclAtom_Sub(&entityCount, 1);
}
