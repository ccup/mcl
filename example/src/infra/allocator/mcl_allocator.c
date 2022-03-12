#include "allocator/mcl_allocator.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/lock/lockobj.h"
#include "mcl/mem/memory.h"
#include "mcl/log/log.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE void* MclFactoryAllocator_AllocOnHeap(MclFactoryAllocator *self, MclSize size) {
	return Mcl_Malloc(size);
}

MCL_PRIVATE void MclFactoryAllocator_FreeOnHeap(MclFactoryAllocator *self, void *obj) {
	Mcl_Free(obj);
}

MCL_PRIVATE void MclFactoryAllocator_InitForHeap(MclFactoryAllocator *self) {
	self->alloc = MclFactoryAllocator_AllocOnHeap;
	self->free = MclFactoryAllocator_FreeOnHeap;
}

///////////////////////////////////////////////////////////
MCL_PRIVATE void* MclFactoryAllocator_AllocLockObj(MclFactoryAllocator *self, MclSize size) {
	return MclLockObj_Create(size);
}

MCL_PRIVATE void MclFactoryAllocator_FreeLockObj(MclFactoryAllocator *self, void *obj) {
	MclLockObj_Delete(obj, self->destroyObj, self->ctxt);
}

MCL_PRIVATE void MclFactoryAllocator_InitForLockObj(MclFactoryAllocator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self->destroyObj);
	self->alloc = MclFactoryAllocator_AllocLockObj;
	self->free = MclFactoryAllocator_FreeLockObj;
}

///////////////////////////////////////////////////////////
MCL_PRIVATE void* MclFactoryAllocator_AllocSharedPtr(MclFactoryAllocator *self, MclSize size) {
	return MclSharedPtr_Create(size, self->destroyObj, self->ctxt);
}

MCL_PRIVATE void MclFactoryAllocator_FreeSharedPtr(MclFactoryAllocator *self, void *obj) {
	MclSharedPtr_Delete(obj);
}

MCL_PRIVATE void MclFactoryAllocator_InitForSharedPtr(MclFactoryAllocator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self->destroyObj);
	self->alloc = MclFactoryAllocator_AllocSharedPtr;
	self->free = MclFactoryAllocator_FreeSharedPtr;
}

///////////////////////////////////////////////////////////
MCL_PRIVATE void MclFactoryAllocator_InitForPool(MclFactoryAllocator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self->alloc);
	MCL_ASSERT_VALID_PTR_VOID(self->free);
	MCL_ASSERT_VALID_PTR_VOID(self->ctxt);
}

///////////////////////////////////////////////////////////
void MclFactoryAllocator_Init(MclFactoryAllocator *self, MclAllocatorType type) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_TRUE_VOID(MclAllocatorType_IsValid(type));

	switch (type) {
	case MCL_ALLOCATOR_HEAP:
		MclFactoryAllocator_InitForHeap(self);
		break;
	case MCL_ALLOCATOR_LOCK_OBJ:
		MclFactoryAllocator_InitForLockObj(self);
		break;
	case MCL_ALLOCATOR_SHARED_PTR:
		MclFactoryAllocator_InitForSharedPtr(self);
		break;
	case MCL_ALLOCATOR_POOL:
		MclFactoryAllocator_InitForPool(self);
		break;
	default:
		MCL_LOG_ERR("Unknown allocator type (%u)", type);
	}
}
