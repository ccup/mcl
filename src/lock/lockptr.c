#include "mcl/lock/lockptr.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE void MclLockPtr_DestroyImpl(MclLockPtr *self, MclLockPtrDeleter ptrDeleter, void *arg) {
    MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Destroy(&self->mutex));
    if (ptrDeleter && self->ptr) ptrDeleter(self->ptr, arg);
    self->ptr = NULL;
}

MCL_PRIVATE void MclLockPtr_UniqueDestroyImpl(MclLockPtr *self, MclLockPtrDeleter ptrDeleter, void *arg) {
    MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Lock(&self->mutex));
    MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Unlock(&self->mutex));
    MclLockPtr_DestroyImpl(self, ptrDeleter, arg);
}

MclStatus MclLockPtr_Init(MclLockPtr *self, void *ptr) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(ptr);
    MCL_ASSERT_SUCC_CALL(MclMutex_Init(&self->mutex, NULL));
    self->ptr = ptr;
    return MCL_SUCCESS;
}

void MclLockPtr_Destroy(MclLockPtr *self, MclLockPtrDeleter ptrDeleter, void *arg) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_DestroyImpl(self, ptrDeleter, arg);
}

void MclLockPtr_UniqueDestroy(MclLockPtr *self, MclLockPtrDeleter ptrDeleter, void *arg) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_UniqueDestroyImpl(self, ptrDeleter, arg);
}

MclLockPtr* MclLockPtr_Create(void *ptr) {
    MclLockPtr *self = MCL_MALLOC(sizeof(MclLockPtr));
    MCL_ASSERT_VALID_PTR_NIL(self);
    MclLockPtr_Init(self, ptr);
    return self;
}

void MclLockPtr_Delete(MclLockPtr *self, MclLockPtrDeleter ptrDeleter, void *arg) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_UniqueDestroyImpl(self, ptrDeleter, arg);
    MCL_FREE(self);
}

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
