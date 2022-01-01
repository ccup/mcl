#include "mcl/array/array.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_PRIVATE uint8_t* MclArray_GetAddr(MclArray *self, uint16_t index) {
    return self->buff + self->elemBytes * index;
}

MclArray* MclArray_Create(uint16_t capacity, uint16_t elemBytes) {
    MCL_ASSERT_TRUE_NIL(capacity > 0);
    MCL_ASSERT_TRUE_NIL(elemBytes > 0);

    MclArray *self = MCL_MALLOC(sizeof(MclArray));
    MCL_ASSERT_VALID_PTR_NIL(self);

    uint8_t *buff = MCL_MALLOC(MclArray_GetBuffSize(capacity, elemBytes));
    if (!buff) {
        MCL_LOG_ERR("Malloc array buff failed!");
        MCL_FREE(self);
        return NULL;
    }

    if (MCL_FAILED(MclArray_Init(self, capacity, elemBytes, buff))) {
        MCL_LOG_ERR("Init array failed!");
        MCL_FREE(buff);
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclArray_Delete(MclArray *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (self->buff) {
        MCL_FREE(self->buff);
    }
    MCL_FREE(self);
}

MclStatus MclArray_Init(MclArray *self, uint16_t capacity, uint16_t elemBytes, uint8_t* buff) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(buff);
    MCL_ASSERT_TRUE(capacity > 0);
    MCL_ASSERT_TRUE(elemBytes > 0);

    self->elemBytes = elemBytes;
    self->capacity = capacity;
    self->buff = buff;
    return MCL_SUCCESS;
}

void MclArray_Clear(MclArray *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    memset(self->buff, 0, MclArray_GetBuffSize(self->capacity, self->elemBytes));
}

void* MclArray_Get(MclArray *self, uint16_t index) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    MCL_ASSERT_TRUE_NIL(index < self->capacity);

    return MclArray_GetAddr(self, index);
}

MclStatus MclArray_Set(MclArray *self, uint16_t index, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(index < self->capacity);

    memcpy(MclArray_GetAddr(self, index), value, self->elemBytes);
    return MCL_SUCCESS;
}

void MclArray_Reset(MclArray *self, uint16_t index) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_TRUE_VOID(index < self->capacity);

    memset(MclArray_GetAddr(self, index), 0, self->elemBytes);
}
