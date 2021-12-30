#include "mcl/lock/lockobj.h"
#include "mcl/lock/mutex.h"
#include "mcl/mem/malloc.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

static const uint64_t MCL_LOCK_OBJ_SENTINEL = 0xdeadc0de;

MCL_TYPE(MclLockObj) {
	uint64_t sentinel;
	MclMutex mutex;
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
	MCL_ASSERT_SUCC_CALL(MclMutex_InitRecursive(&self->mutex));
	self->sentinel = MCL_LOCK_OBJ_SENTINEL;
	self->ptr = (uint8_t*)self + MclLockObj_HeaderSize();
	return MCL_SUCCESS;
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

    (void)MclLockObj_Lock(obj);
    (void)MclLockObj_Unlock(obj);

	if(objDtor) objDtor(obj, arg);
	MCL_FREE(self);
}

MclStatus MclLockObj_Lock(void *obj) {
    MCL_ASSERT_VALID_PTR(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR(self);

	MCL_ASSERT_SUCC_CALL(MclMutex_Lock(&self->mutex));
	return MCL_SUCCESS;
}

MclStatus MclLockObj_Unlock(void *obj) {
    MCL_ASSERT_VALID_PTR(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR(self);

	MCL_ASSERT_SUCC_CALL(MclMutex_Unlock(&self->mutex));
	return MCL_SUCCESS;
}
