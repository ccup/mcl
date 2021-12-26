#include "mcl/task/lockptr.h"
#include "mcl/mem/malloc.h"

MclStatus MclLockPtr_Lock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_SUCC_CALL(MclMutex_Lock(&self->mutex));
	return MCL_SUCCESS;
}

MclStatus MclLockPtr_Unlock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_SUCC_CALL(MclMutex_Unlock(&self->mutex));
	return MCL_SUCCESS;
}

MclStatus MclLockPtr_Init(MclLockPtr *self, void *ptr) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(ptr);
    MCL_ASSERT_SUCC_CALL(MclMutex_InitRecursive(&self->mutex));
    self->ptr = ptr;
    return MCL_SUCCESS;
}

void MclLockPtr_Destroy(MclLockPtr *self, MclLockPtr_PtrDeleter ptrDeleter) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Destroy(&self->mutex));
    if (ptrDeleter && self->ptr) ptrDeleter(self->ptr);
    self->ptr = NULL;
}

void MclLockPtr_UniqueDestroy(MclLockPtr *self, MclLockPtr_PtrDeleter ptrDeleter) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_SUCC_CALL_VOID(MclLockPtr_Lock(self));
    MCL_ASSERT_SUCC_CALL_VOID(MclLockPtr_Unlock(self));
    MclLockPtr_Destroy(self, ptrDeleter);
}

MclLockPtr* MclLockPtr_Create(void *ptr) {
    MclLockPtr *self = MCL_MALLOC(sizeof(MclLockPtr));
    MCL_ASSERT_VALID_PTR_NIL(self);
    MclLockPtr_Init(self, ptr);
    return self;
}

void MclLockPtr_Delete(MclLockPtr *self, MclLockPtr_PtrDeleter ptrDeleter) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_UniqueDestroy(self, ptrDeleter);
    MCL_FREE(self);
}
