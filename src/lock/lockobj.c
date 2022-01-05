#include "mcl/lock/lockobj.h"
#include "mcl/lock/rwlock.h"
#include "mcl/mem/malloc.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

static const uint64_t MCL_LOCK_OBJ_SENTINEL = 0xdeadc0de;

MCL_TYPE(MclLockObj) {
	uint64_t sentinel;
	MclRwLock rwlock;
	void *ptr;
};

MCL_PRIVATE size_t MclLockObj_HeaderSize() {
    return MclAlign_GetSizeOf(sizeof(MclLockObj));
}

MCL_PRIVATE bool MclLockObj_IsValid(MclLockObj* self, void *obj) {
	return (self && self->sentinel == MCL_LOCK_OBJ_SENTINEL && self->ptr == obj);
}

MCL_PRIVATE MclLockObj* MclLockObj_GetSelf(void *obj) {
	MclLockObj *self = (MclLockObj*)((uint8_t*)obj - MclLockObj_HeaderSize());
	return MclLockObj_IsValid(self, obj) ? self : NULL;
}

MCL_PRIVATE MclStatus MclLockObj_Init(MclLockObj *self) {
	MCL_ASSERT_SUCC_CALL(MclRwLock_Init(&self->rwlock, NULL));
	self->sentinel = MCL_LOCK_OBJ_SENTINEL;
	self->ptr = (uint8_t*)self + MclLockObj_HeaderSize();
	return MCL_SUCCESS;
}

MCL_PRIVATE void MclLockObj_DestroyObj(MclLockObj *self, MclLockObjDestructor objDtor, void *arg) {
	MCL_LOCK_WRITE_AUTO(self->rwlock);
	if(objDtor) objDtor(self->ptr, arg);
}

MCL_PRIVATE void MclLockObj_Destroy(MclLockObj *self, MclLockObjDestructor objDtor, void *arg) {
    MclLockObj_DestroyObj(self, objDtor, arg);
    MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Destroy(&self->rwlock));
}

void* MclLockObj_Create(size_t size) {
	MCL_ASSERT_TRUE_NIL(size > 0);

	MclLockObj *self = MCL_MALLOC(MclLockObj_HeaderSize() + MclAlign_GetSizeOf(size));
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclLockObj_Init(self))) {
		MCL_LOG_ERR("Init lock obj failed!");
		MCL_FREE(self);
		return NULL;
	}

	return self->ptr;
}

void  MclLockObj_Delete(void *obj, MclLockObjDestructor objDtor, void *arg) {
	MCL_ASSERT_VALID_PTR_VOID(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR_VOID(self);

    MclLockObj_Destroy(self, objDtor, arg);
	MCL_FREE(self);
}

MclStatus MclLockObj_RdLock(void *obj) {
    MCL_ASSERT_VALID_PTR(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR(self);

	MCL_ASSERT_SUCC_CALL(MclRwLock_RdLock(&self->rwlock));
	return MCL_SUCCESS;
}

MclStatus MclLockObj_WrLock(void *obj) {
    MCL_ASSERT_VALID_PTR(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR(self);

	MCL_ASSERT_SUCC_CALL(MclRwLock_WrLock(&self->rwlock));
	return MCL_SUCCESS;
}

MclStatus MclLockObj_UnLock(void *obj) {
    MCL_ASSERT_VALID_PTR(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR(self);

	MCL_ASSERT_SUCC_CALL(MclRwLock_UnLock(&self->rwlock));
	return MCL_SUCCESS;
}
