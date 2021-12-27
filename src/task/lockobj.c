#include "mcl/task/lockobj.h"
#include "mcl/task/mutex.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

static const uint64_t MCL_LOCK_OBJ_SENTINEL = 0xdeadc0de;

MCL_TYPE(MclLockObj) {
	uint64_t sentinel;
	MclMutex mutex;
	uint8_t obj[];
};

MCL_PRIVATE bool MclLockObj_IsValid(MclLockObj* self) {
	return (self && self->sentinel == MCL_LOCK_OBJ_SENTINEL);
}

MCL_PRIVATE MclLockObj* MclLockObj_GetSelf(void *obj) {
	uint8_t *ptr = (uint8_t*)obj;
	MclLockObj *self = (MclLockObj*)(ptr - sizeof(MclLockObj));
	return MclLockObj_IsValid(self) ? self : NULL;
}

MCL_PRIVATE MclStatus MclLockObj_Init(MclLockObj *self) {
	MCL_ASSERT_SUCC_CALL(MclMutex_InitRecursive(&self->mutex));
	self->sentinel = MCL_LOCK_OBJ_SENTINEL;
	return MCL_SUCCESS;
}

void* MclLockObj_Create(uint32_t size) {
	MCL_ASSERT_TRUE_NIL(size > 0);

	MclLockObj *self = MCL_MALLOC(sizeof(MclLockObj) + size);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclLockObj_Init(self))) {
		MCL_LOG_ERR("Init lock obj failed!");
		MCL_FREE(self);
		return NULL;
	}

	return self->obj;
}

void  MclLockObj_Delete(void *obj, MclLockObjDestructor objDtor, void *arg) {
	MCL_ASSERT_VALID_PTR_VOID(obj);

	MclLockObj *self = MclLockObj_GetSelf(obj);
	MCL_ASSERT_VALID_PTR_VOID(self);

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
