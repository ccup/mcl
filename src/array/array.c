#include "mcl/array/array.h"
#include "mcl/mem/malloc.h"

MCL_PRIVATE uint8_t* MclArray_GetAddr(MclArray *self, uint16_t index) {
    return self->buff + self->elemBytes * index;
}

MclArray* MclArray_Create(uint16_t count, uint16_t elemBytes) {
    MCL_ASSERT_TRUE_NIL(count > 0);
    MCL_ASSERT_TRUE_NIL(elemBytes > 0);

    MclArray *self = MCL_MALLOC(sizeof(MclArray));
    MCL_ASSERT_VALID_PTR_NIL(self);

    uint8_t *buff = MCL_MALLOC(MclArray_GetBuffSize(count, elemBytes));
    if (!buff) {
        MCL_LOG_ERR("Malloc array buff failed!");
        MCL_FREE(self);
        return NULL;
    }

    MclArray_Init(self, count, elemBytes, buff);
    return self;
}

void MclArray_Delete(MclArray *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (self->buff) {
        MCL_FREE(self->buff);
    }
    MCL_FREE(self);
}

void MclArray_Init(MclArray *self, uint16_t count, uint16_t elemBytes, uint8_t* buff) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_VALID_PTR_VOID(buff);
    MCL_ASSERT_TRUE_VOID(count > 0);
    MCL_ASSERT_TRUE_VOID(elemBytes > 0);

    self->elemBytes = elemBytes;
    self->count = count;
    self->buff = buff;
}

void MclArray_Clear(MclArray *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    memset(self->buff, 0, MclArray_GetBuffSize(self->count, self->elemBytes));
}

uint8_t* MclArray_Get(MclArray *self, uint16_t index) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(index < self->count);

    return MclArray_GetAddr(self, index);
}

MclStatus MclArray_Set(MclArray *self, uint16_t index, uint8_t *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(index < self->count);

    memcpy(MclArray_GetAddr(self, index), value, self->elemBytes);
    return MCL_SUCCESS;
}

void MclArray_Reset(MclArray *self, uint16_t index) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_TRUE_VOID(index < self->count);

    memset(MclArray_GetAddr(self, index), 0, self->elemBytes);
}
