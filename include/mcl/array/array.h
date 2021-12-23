#ifndef MCL_19AD2FB4B0334BD1A90444373D4F9AC3
#define MCL_19AD2FB4B0334BD1A90444373D4F9AC3

#include "mcl/typedef.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

MCL_TYPE(MclArray) {
    uint16_t count;
    uint16_t elemBytes;
    uint8_t *buff;
};

MclArray* MclArray_Create(uint16_t count, uint16_t elemBytes);
void MclArray_Delete(MclArray*);

void MclArray_Init(MclArray*, uint16_t count, uint16_t elemBytes, uint8_t* buff);
void MclArray_Clear(MclArray*);

uint8_t*  MclArray_Get(MclArray*, uint16_t index);
MclStatus MclArray_Set(MclArray*, uint16_t index, uint8_t *value);

void MclArray_Reset(MclArray*, uint16_t index);

/////////////////////////////////////////////////////////////////////
MCL_PRIVATE uint32_t MclArray_GetBuffSize(uint16_t count, uint16_t elemBytes) {
    return sizeof(uint8_t) * count * elemBytes;
}

MCL_INLINE uint8_t* MclArray_Begin(MclArray *self) {
    return self->buff;
}

MCL_INLINE uint8_t* MclArray_End(MclArray *self) {
    return self->buff + MclArray_GetBuffSize(self->count, self->elemBytes);
}

#define MCL_ARRAY_FOREACH(ARRAY, TYPE, PTR) \
for (PTR  = (TYPE*)MclArray_Begin(ARRAY); PTR < (TYPE*)MclArray_End(ARRAY); PTR++)

MCL_STDC_END

#endif
