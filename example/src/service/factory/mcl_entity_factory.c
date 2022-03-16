#include "factory/mcl_entity_factory.h"
#include "allocator/mcl_allocator.h"
#include "entity/private/mcl_entity_private.h"
#include "entity/mcl_entity.h"
#include "mcl/mem/allocator.h"
#include "mcl/lock/atomic.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE const MclSize MCL_ENTITY_CAPACITY = 16;

MCL_ALLOCATOR_TYPE_DEF(MclEntityAllocator, MclEntity, MCL_ENTITY_CAPACITY);

MCL_PRIVATE MclEntityAllocator entityAllocator;

MCL_PRIVATE void* MclEntityFactory_PoolAlloc(MclFactoryAllocator *allocator, MclSize size) {
	MclEntityAllocator *poolAllocator = (MclEntityAllocator*)(allocator->ctxt);
	return MclEntityAllocator_Alloc(poolAllocator);
}

MCL_PRIVATE void MclEntityFactory_PoolFree(MclFactoryAllocator *allocator, void *obj) {
	MclEntityAllocator *poolAllocator = (MclEntityAllocator*)(allocator->ctxt);
	MclEntityAllocator_Free(poolAllocator, (MclEntity*)obj);
}

void MclEntityFactory_InitPoolAllocator(MclFactoryAllocator *allocator) {
	MCL_ALLOCATOR_INIT(MclEntityAllocator, entityAllocator);

	allocator->alloc = MclEntityFactory_PoolAlloc;
	allocator->free = MclEntityFactory_PoolFree;
	allocator->ctxt = &entityAllocator;
}

///////////////////////////////////////////////////////////
typedef struct {
	MclFactoryAllocator allocator;
	MclAtomic count;
} MclEntityFactory;

MCL_PRIVATE MclEntityFactory factory;

MCL_PRIVATE void MclEntityFactory_DestroyEntity(void *obj, void *arg) {
	MclEntity_Destroy((MclEntity*)obj);
}

MCL_PRIVATE void MclEntityFactory_InitAllocator(MclFactoryAllocator *allocator, MclAllocatorType type) {
	if (type == MCL_ALLOCATOR_POOL) {
		MclEntityFactory_InitPoolAllocator(allocator);
	}

	allocator->destroyObj = MclEntityFactory_DestroyEntity;
	allocator->ctxt = NULL;
	MclFactoryAllocator_Init(allocator, type);
}

void MclEntityFactory_Init(MclAllocatorType type) {
	MclEntityFactory_InitAllocator(&factory.allocator, type);
	MclAtomic_Clear(&factory.count);
}

MclEntity* MclEntityFactory_Create(MclEntityId id, void *cfg) {
	MclEntity *entity = MclFactoryAllocator_Create(&factory.allocator, sizeof(MclEntity));
	MCL_ASSERT_VALID_PTR_NIL(entity);

	if (MCL_FAILED(MclEntity_Init(entity, id, cfg))) {
		MCL_LOG_ERR("Initialize entity (%u) failed!", id);
		MclFactoryAllocator_Delete(&factory.allocator, entity);
		return NULL;
	}

	MclAtomic_AddFetch(&factory.count, 1);
	return entity;
}

void MclEntityFactory_Delete(MclEntity *entity) {
	MCL_ASSERT_VALID_PTR_VOID(entity);
	MclFactoryAllocator_Delete(&factory.allocator, entity);
	MclAtomic_SubFetch(&factory.count, 1);
}


MclSize MclEntityFactory_GetUnreleasedCount() {
	return factory.count;
}
