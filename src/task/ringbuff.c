#include "mcl/task/ringbuff.h"
#include "mcl/mem/malloc.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

MCL_PRIVATE uint16_t MclRingBuff_GetNextPos(const MclRingBuff *self, uint16_t pos) {
    return (pos + 1) % MclArray_GetCount(&self->buff);
}

MCL_PRIVATE uint16_t MclRingBuff_GetNextHead(const MclRingBuff *self) {
    return MclRingBuff_GetNextPos(self, (uint16_t)self->head);
}

MCL_PRIVATE uint16_t MclRingBuff_GetNextTail(const MclRingBuff *self) {
    return MclRingBuff_GetNextPos(self, (uint16_t)self->tail);
}

MclRingBuff* MclRingBuff_Create(uint16_t count, uint16_t elemBytes) {
    MCL_ASSERT_TRUE_NIL(count > 0);
    MCL_ASSERT_TRUE_NIL(elemBytes > 0);

    MclRingBuff *self = MCL_MALLOC(sizeof(MclRingBuff));
    MCL_ASSERT_VALID_PTR_NIL(self);

    elemBytes = MclAlign_GetSizeOf(elemBytes);

    uint8_t *buff = MCL_MALLOC(MclArray_GetBuffSize(count, elemBytes));
    if (!buff) {
        MCL_LOG_ERR("Malloc array for ringbuff failed!");
        MCL_FREE(self);
        return NULL;
    }

    (void)MclRingBuff_Init(self, count, elemBytes, buff);
    return self;
}

void MclRingBuff_Delete(MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (self->buff.buff) {
        MCL_FREE(self->buff.buff);
    }
    MCL_FREE(self);
}

MclStatus MclRingBuff_Init(MclRingBuff *self, uint16_t count, uint16_t elemBytes, uint8_t* buff) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(buff);
    MCL_ASSERT_TRUE(count > 0);
    MCL_ASSERT_TRUE(elemBytes > 0);

    MclArray_Init(&self->buff, count + 1, elemBytes, buff);
    self->head = 0;
    self->tail = 0;
    return MCL_SUCCESS;
}

void MclRingBuff_Reset(MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclArray_Clear(&self->buff);
    self->head = 0;
    self->tail = 0;
}

bool MclRingBuff_IsFull(const MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_BOOL(self);
    return MclRingBuff_GetNextTail(self) == self->head;
}

bool MclRingBuff_IsEmpty(const MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);
    return self->head == self->tail;
}

MclStatus MclRingBuff_Pop(MclRingBuff *self, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    if (MclRingBuff_IsEmpty(self)) return MCL_FAILURE;

    void *result = MclArray_Get(&self->buff, self->head);
    MCL_ASSERT_VALID_PTR(result);

    memcpy(value, result, MclArray_GetElemSize(&self->buff));
    self->head = MclRingBuff_GetNextHead(self);
    return MCL_SUCCESS;
}

MclStatus MclRingBuff_Put(MclRingBuff *self, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    if (MclRingBuff_IsFull(self)) return MCL_FAILURE;

    MCL_ASSERT_SUCC_CALL(MclArray_Set(&self->buff, self->tail, value));
    self->tail = MclRingBuff_GetNextTail(self);
    return MCL_SUCCESS;
}
