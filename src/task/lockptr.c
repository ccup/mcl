#include "mcl/task/lockptr.h"
#include "mcl/mem/malloc.h"

MclStatus MclLockPtr_Lock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
    return MclMutex_Lock(&self->mutex);
}

MclStatus MclLockPtr_Unlock(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR(self);
    return MclMutex_Unlock(&self->mutex);
}

MclStatus MclLockPtr_Init(MclLockPtr *self, void *ptr, MclLockPtrDeleter ptrDeleter) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(ptr);
    MCL_ASSERT_SUCC_CALL(MclMutex_InitRecursive(&self->mutex));
    self->ptr = ptr;
    self->deleter = ptrDeleter;
    return MCL_SUCCESS;
}

void MclLockPtr_Destroy(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Destroy(&self->mutex));
    if (self->deleter && self->ptr) self->deleter(self->ptr);
    self->ptr = NULL;
}

void MclLockPtr_UniqueDestroy(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_SUCC_CALL_VOID(MclLockPtr_Lock(self));
    MCL_ASSERT_SUCC_CALL_VOID(MclLockPtr_Unlock(self));
    MclLockPtr_Destroy(self);
}

MclLockPtr* MclLockPtr_Create(void *ptr, MclLockPtrDeleter ptrDeleter) {
    MclLockPtr *self = MCL_MALLOC(sizeof(MclLockPtr));
    MCL_ASSERT_VALID_PTR_NIL(self);
    MclLockPtr_Init(self, ptr, ptrDeleter);
    return self;
}

void MclLockPtr_Delete(MclLockPtr *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclLockPtr_UniqueDestroy(self);
    MCL_FREE(self);
}
