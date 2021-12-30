#ifndef MCL_19AD2FB4B0334BD1A90444373D4F9AC3
#define MCL_19AD2FB4B0334BD1A90444373D4F9AC3

#include "mcl/typedef.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE(MclArray) {
    uint16_t capacity;
    uint16_t elemBytes;
    uint8_t *buff;
};

MclArray* MclArray_Create(uint16_t capacity, uint16_t elemBytes);
void MclArray_Delete(MclArray*);

MclStatus MclArray_Init(MclArray*, uint16_t capacity, uint16_t elemBytes, uint8_t* buff);
void MclArray_Clear(MclArray*);

void* MclArray_Get(MclArray*, uint16_t index);
MclStatus MclArray_Set(MclArray*, uint16_t index, void*);

void MclArray_Reset(MclArray*, uint16_t index);

/////////////////////////////////////////////////////////////////////
MCL_INLINE uint16_t MclArray_GetCapacity(const MclArray *self) {
    return self ? self->capacity : 0;
}

MCL_INLINE uint16_t MclArray_GetElemSize(const MclArray *self) {
    return self ? self->elemBytes : 0;
}

MCL_INLINE uint32_t MclArray_GetBuffSize(uint16_t capacity, uint16_t elemBytes) {
    return sizeof(uint8_t) * capacity * elemBytes;
}

MCL_INLINE uint8_t* MclArray_Begin(MclArray *self) {
    return self ? self->buff : NULL;
}

MCL_INLINE uint8_t* MclArray_End(MclArray *self) {
    return self ? self->buff + MclArray_GetBuffSize(self->capacity, self->elemBytes) : NULL;
}

#define MCL_ARRAY_FOREACH(ARRAY, TYPE, PTR)     \
for (PTR  = (TYPE*)MclArray_Begin(ARRAY); PTR < (TYPE*)MclArray_End(ARRAY); PTR++)

#define MCL_ARRAY_FOREACH_INDEX(ARRAY, INDEX)   \
for (INDEX = 0; INDEX < (ARRAY)->capacity; INDEX++)

#define MCL_ARRAY(CAPACITY, ELEM_BYTES, BUFF) \
{.capacity = CAPACITY, .elemBytes = ELEM_BYTES, .buff = BUFF}

MCL_STDC_END

#endif
