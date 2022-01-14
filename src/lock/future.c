#include "mcl/lock/future.h"
#include "mcl/lock/atom.h"
#include "mcl/lock/mutex.h"
#include "mcl/lock/cond.h"
#include "mcl/mem/malloc.h"

MCL_TYPE(MclFuture) {
    MclMutex mutex;
    MclCond cond;
    MclAtom isReady;
    MclStatus status;
    void *value;
};

MCL_PRIVATE MclStatus MclFuture_Init(MclFuture *self) {
    if (MCL_FAILED(MclMutex_Init(&self->mutex, NULL))) {
        MCL_LOG_ERR("Init mutex failed!");
        return MCL_FAILURE;
    }
    if (MCL_FAILED(MclCond_Init(&self->cond, NULL))) {
        MCL_LOG_ERR("Init cond failed!");
        (void)MclMutex_Destroy(&self->mutex);
        return MCL_FAILURE;
    }
    MclAtom_Clear(&self->isReady);
    self->status = MCL_UNINITIALIZED;
    self->value = NULL;
    return MCL_SUCCESS;
}

MCL_PRIVATE void MclFuture_Destroy(MclFuture *self) {
    MCL_PEEK_SUCC_CALL(MclMutex_Destroy(&self->mutex));
    MCL_PEEK_SUCC_CALL(MclCond_Destroy(&self->cond));
    MclAtom_Clear(&self->isReady);
}

MclFuture* MclFuture_Create() {
    MclFuture *self = MCL_MALLOC(sizeof(MclFuture));
    MCL_ASSERT_VALID_PTR_NIL(self);

    if (MCL_FAILED(MclFuture_Init(self))) {
        MCL_LOG_ERR("Init future failed!");
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclFuture_Delete(MclFuture *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclFuture_Destroy(self);
    MCL_FREE(self);
}

void MclFuture_Stop(MclFuture *self) {
    if (MclAtom_IsTrue(&self->isReady)) return;

    MCL_LOCK_AUTO(self->mutex);
    MclAtom_Set(&self->isReady, 1);
    self->status = MCL_STATUS_DONE;
    MclCond_Broadcast(&self->cond);
}

bool MclFuture_IsReady(const MclFuture *self) {
    return MclAtom_IsTrue(&((MclFuture*)self)->isReady);
}

void MclFuture_Set(MclFuture *self, MclStatus status, void *value) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_TRUE_VOID(!MclAtom_IsTrue(&self->isReady));

    MCL_LOCK_AUTO(self->mutex);

    MclAtom_Set(&self->isReady, 1);
    self->status = status;
    self->value = value;
    MclCond_Signal(&self->cond);
}

void MclFuture_Get(MclFuture *self, MclStatus *status, void **value) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MCL_LOCK_AUTO(self->mutex);
    while (!MclAtom_IsTrue(&self->isReady)) {
        MclCond_Wait(&self->cond, &self->mutex);
    }
    *status = self->status;
    (*value) = self->value;
}
