#include "mcl/msg/msg_queue.h"
#include "mcl/mem/malloc.h"

MclMsgQueue* MclMsgQueue_Create(uint16_t capacity) {
    MCL_ASSERT_TRUE_NIL(capacity > 0);

    MclMsgQueue *self = MCL_MALLOC(sizeof(MclMsgQueue));
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclMsg *msgBuff = MCL_MALLOC(sizeof(MclMsg) * capacity);
    if (!msgBuff) {
        MCL_LOG_ERR("malloc memory of msg buff failed!");
        MCL_FREE(self);
        return NULL;
    }

    if (MCL_FAILED(MclMsgQueue_Init(self, capacity + 1, msgBuff))) {
        MCL_LOG_ERR("Init msg queue failed!");
        MCL_FREE(msgBuff);
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclMsgQueue_Delete(MclMsgQueue *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    void *buff = MclRingBuff_GetBuff(&self->ringbuff);
    if (buff) MCL_FREE(buff);
    MCL_FREE(self);
}

MclStatus MclMsgQueue_Init(MclMsgQueue *self, uint16_t capacity, MclMsg* msgBuff) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(msgBuff);
    MCL_ASSERT_TRUE(capacity > 1);

    MCL_ASSERT_SUCC_CALL(MclRingBuff_Init(&self->ringbuff, capacity, sizeof(MclMsg), (uint8_t*)msgBuff));
    MCL_ASSERT_SUCC_CALL(MclMutex_Init(&self->mutex, NULL));

    return MCL_SUCCESS;
}

void MclMsgQueue_Clear(MclMsgQueue *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_LOCK_AUTO(self->mutex);
    MclRingBuff_Reset(&self->ringbuff);
}

bool MclMsgQueue_IsFull(const MclMsgQueue *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);
    MCL_LOCK_AUTO(self->mutex);
    return MclRingBuff_IsFull(&self->ringbuff);
}

bool MclMsgQueue_IsEmpty(const MclMsgQueue *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);
    MCL_LOCK_AUTO(self->mutex);
    return MclRingBuff_IsEmpty(&self->ringbuff);
}

uint16_t MclMsgQueue_GetCount(const MclMsgQueue *self) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_LOCK_AUTO(self->mutex);
    return MclRingBuff_GetCount(&self->ringbuff);
}

MclStatus MclMsgQueue_Send(MclMsgQueue *self, MclMsg *msg) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(msg);

    MCL_LOCK_AUTO(self->mutex);
    return MclRingBuff_Put(&self->ringbuff, msg);
}

MclStatus MclMsgQueue_Recv(MclMsgQueue *self, MclMsg *result) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(result);

    MCL_LOCK_AUTO(self->mutex);

    MclMsg msg;
    if (MCL_FAILED(MclRingBuff_Pop(&self->ringbuff, &msg))) {
        return MCL_FAILURE;
    }
    MCL_ASSERT_SUCC_CALL(MclMsg_Copy(&msg, result));
    return MCL_SUCCESS;
}