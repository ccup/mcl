#include "factory/mcl_entity_factory.h"
#include "mcl/service/config/mcl_entity_config.h"
#include "entity/mcl_entity.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/mem/allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/lock/lockobj.h"
#include "mcl/lock/atom.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE MclAtom entityCount = 0;

size_t MclEntityFactory_GetUnreleasedCount() {
	return entityCount;
}

///////////////////////////////////////////////////////////
MclEntity* MclEntityFactory_Create(MclEntityId id, const MclEntityConfig *config) {
	MclEntity *self = MCL_MALLOC(MCL_ENTITY_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id, config->value))) {
		MCL_LOG_ERR("Initialize entity (%u) failed!", id);
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
MCL_PRIVATE void MclEntityFactory_DestroyEntity(void *obj, void *arg) {
	MclEntity_Destroy((MclEntity*)obj);
}

MclEntity* MclEntityFactory_CreateSharedPtr(MclEntityId id, const MclEntityConfig *config) {
	MclEntity *self = MclSharedPtr_Create(MCL_ENTITY_SIZE, MclEntityFactory_DestroyEntity, NULL);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id, config->value))) {
		MCL_LOG_ERR("Initialize shared ptr (%u) of entity failed!", id);
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
MclEntity* MclEntityFactory_CreateLockObj(MclEntityId id, const MclEntityConfig *config) {
	MclEntity *self = (MclEntity*)MclLockObj_Create(MCL_ENTITY_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclEntity_Init(self, id, config->value))) {
		MCL_LOG_ERR("Initialize lock entity (%u) failed!", id);
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

/////////////////////////////////////////////////////////
//#define MCL_ENTITY_MEM_SIZE  8
//MCL_PRIVATE const uint16_t MCL_ENTITY_CAPACITY = 16;
//
//MCL_ALLOCATOR_TYPE_DEF(MclEntityAllocator, MclEntity, MCL_ENTITY_CAPACITY);
//
//MCL_PRIVATE MclEntityAllocator entityAllocator;
//
//MCL_CTOR void MclEntityAllocator_Ctor() {
//	if (MCL_ENTITY_MEM_SIZE < MCL_ENTITY_SIZE) {
//		MCL_LOG_FATAL("Size (%u) of entity memory in allocator is less than actual size (%lu)!"
//				, MCL_ENTITY_MEM_SIZE, MCL_ENTITY_SIZE);
//	}
//	MCL_ALLOCATOR_INIT(MclEntityAllocator, entityAllocator);
//	MCL_LOG_SUCC("Entity allocator init OK!");
//}
//
//MclEntity* MclEntityFactory_CreateStatic(MclEntityId id, const MclEntityConfig *config) {
//	if (MCL_ENTITY_MEM_SIZE < MCL_ENTITY_SIZE) return NULL;
//
//	MclEntity* self = MCL_ALLOCATOR_ALLOC(MclEntityAllocator, entityAllocator);
//	MCL_ASSERT_VALID_PTR_NIL(self);
//
//	if (MCL_FAILED(MclEntity_Init(self, id, config->value))) {
//		MCL_LOG_ERR("Initialize static entity (%u) failed!", id);
//		MclEntityAllocator_Free(&entityAllocator, self);
//		return NULL;
//	}
//
//	MclAtom_Add(&entityCount, 1);
//	return self;
//
//}
//
//void MclEntityFactory_DeleteStatic(MclEntity *self) {
//	MCL_ASSERT_VALID_PTR_VOID(self);
//
//	MCL_ALLOCATOR_FREE(MclEntityAllocator, entityAllocator, self);
//	MclAtom_Sub(&entityCount, 1);
//}
