#include "mcl/lock/lockptr.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE void MclLockPtr_DestroyPtr(MclLockPtr *self, MclLockPtrDestroy destroy, void *arg) {
	MCL_LOCK_WRITE_AUTO(self->rwlock);
    if (destroy && self->ptr) destroy(self->ptr, arg);
    self->ptr = NULL;
}

MCL_PRIVATE void MclLockPtr_DestroyImpl(MclLockPtr *self, MclLockPtrDestroy destroy, void *arg) {
	MclLockPtr_DestroyPtr(self, destroy, arg);
	MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Destroy(&self->rwlock));
}

MclStatus MclLockPtr_Init(MclLockPtr *self, void *ptr) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(ptr);
    MCL_ASSERT_SUCC_CALL(MclRwLock_Init(&self->rwlock, NULL));
    self->ptr = ptr;
    return MCL_SUCCESS;
}

void MclLockPtr_Destroy(MclLockPtr *self, MclLockPtrDestroy destroy, void *arg) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_DestroyImpl(self, destroy, arg);
}

MclLockPtr* MclLockPtr_Create(void *ptr) {
    MclLockPtr *self = MCL_MALLOC(sizeof(MclLockPtr));
    MCL_ASSERT_VALID_PTR_NIL(self);
    MclLockPtr_Init(self, ptr);
    return self;
}

void MclLockPtr_Delete(MclLockPtr *self, MclLockPtrDestroy destroy, void *arg) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_DestroyImpl(self, destroy, arg);
    MCL_FREE(self);
}

MclStatus MclLockPtr_WrLock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_SUCC_CALL(MclRwLock_WrLock(&self->rwlock));
    return MCL_SUCCESS;
}

MclStatus MclLockPtr_RdLock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_SUCC_CALL(MclRwLock_RdLock(&self->rwlock));
    return MCL_SUCCESS;
}

MclStatus MclLockPtr_UnLock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_SUCC_CALL(MclRwLock_UnLock(&self->rwlock));
    return MCL_SUCCESS;
}
