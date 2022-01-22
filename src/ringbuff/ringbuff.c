#include "mcl/ringbuff/ringbuff.h"
#include "mcl/mem/memory.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

MCL_PRIVATE MclArrayIndex MclRingBuff_GetNextPos(const MclRingBuff *self, MclArrayIndex pos) {
    return (pos + 1) % MclArray_GetCapacity(&self->buff);
}

MCL_PRIVATE MclArrayIndex MclRingBuff_GetNextHead(const MclRingBuff *self) {
    return MclRingBuff_GetNextPos(self, MclAtom_Get(&((MclRingBuff*)self)->head));
}

MCL_PRIVATE MclArrayIndex MclRingBuff_GetNextTail(const MclRingBuff *self) {
    return MclRingBuff_GetNextPos(self, MclAtom_Get(&((MclRingBuff*)self)->tail));
}

MclRingBuff* MclRingBuff_Create(MclSize capacity, MclSize elemBytes) {
    MCL_ASSERT_TRUE_NIL(capacity > 0);
    MCL_ASSERT_TRUE_NIL(elemBytes > 0);

    MclRingBuff *self = MCL_MALLOC(sizeof(MclRingBuff));
    MCL_ASSERT_VALID_PTR_NIL(self);

    elemBytes = MclAlign_GetSizeOf(elemBytes);

    uint8_t *buff = MCL_MALLOC(MclArray_GetBuffSize(capacity, elemBytes));
    if (!buff) {
        MCL_LOG_ERR("Malloc array for ringbuff failed!");
        MCL_FREE(self);
        return NULL;
    }

    if (MCL_FAILED(MclRingBuff_Init(self, capacity, elemBytes, buff))) {
        MCL_LOG_ERR("Init ringbuff failed!");
        MCL_FREE(buff);
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclRingBuff_Delete(MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    void *buff = MclArray_Begin(&self->buff);
    if (buff) {
        MCL_FREE(buff);
    }
    MCL_FREE(self);
}

MclStatus MclRingBuff_Init(MclRingBuff *self, MclSize capacity, MclSize elemBytes, uint8_t* buff) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(buff);
    MCL_ASSERT_TRUE(capacity > 0);
    MCL_ASSERT_TRUE(elemBytes > 0);

    MclArray_Init(&self->buff, capacity, elemBytes, buff);
    MclAtom_Set(&self->head, 0);
    MclAtom_Set(&self->tail, 0);
    return MCL_SUCCESS;
}

void MclRingBuff_Reset(MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclArray_Clear(&self->buff);
    MclAtom_Clear(&self->head);
    MclAtom_Clear(&self->tail);
}

bool MclRingBuff_IsFull(const MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);
    return MclRingBuff_GetNextTail(self) == MclAtom_Get(&((MclRingBuff*)self)->head);
}

bool MclRingBuff_IsEmpty(const MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);
    return MclAtom_Get(&((MclRingBuff*)self)->head) == MclAtom_Get(&((MclRingBuff*)self)->tail);
}

MclSize MclRingBuff_GetCount(const MclRingBuff *self) {
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclSize capacity = MclArray_GetCapacity(&self->buff);
    return (MclAtom_Get(&((MclRingBuff*)self)->tail) + capacity - MclAtom_Get(&((MclRingBuff*)self)->head)) % capacity;
}

MclStatus MclRingBuff_Pop(MclRingBuff *self, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    if (MclRingBuff_IsEmpty(self)) return MCL_FAILURE;

    void *result = MclArray_Get(&self->buff, MclAtom_Get(&self->head));
    MCL_ASSERT_VALID_PTR(result);

    MCL_MEM_COPY(value, result, MclArray_GetElemSize(&self->buff));
    MclAtom_Set(&self->head, MclRingBuff_GetNextHead(self));
    return MCL_SUCCESS;
}

MclStatus MclRingBuff_Put(MclRingBuff *self, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    if (MclRingBuff_IsFull(self)) return MCL_FAILURE;

    MCL_ASSERT_SUCC_CALL(MclArray_Set(&self->buff, MclAtom_Get(&self->tail), value));
    MclAtom_Set(&self->tail, MclRingBuff_GetNextTail(self));
    return MCL_SUCCESS;
}
