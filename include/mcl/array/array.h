#ifndef MCL_19AD2FB4B0334BD1A90444373D4F9AC3
#define MCL_19AD2FB4B0334BD1A90444373D4F9AC3

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/array/array_index.h"

MCL_STDC_BEGIN

MCL_TYPE(MclArray) {
	MclSize capacity;
	MclSize elemBytes;
    uint8_t *buff;
};

MclArray* MclArray_Create(MclSize capacity, MclSize elemBytes);
void MclArray_Delete(MclArray*);

MclStatus MclArray_Init(MclArray*, MclSize capacity, MclSize elemBytes, uint8_t* buff);
void MclArray_Clear(MclArray*);

void* MclArray_Get(MclArray*, MclArrayIndex index);
MclStatus MclArray_Set(MclArray*, MclArrayIndex index, void*);

void MclArray_Reset(MclArray*, MclArrayIndex index);

///////////////////////////////////////////////////////////
MCL_INLINE MclSize MclArray_GetCapacity(const MclArray *self) {
    return self ? self->capacity : 0;
}

MCL_INLINE MclSize MclArray_GetElemSize(const MclArray *self) {
    return self ? self->elemBytes : 0;
}

MCL_INLINE uint64_t MclArray_GetBuffSize(MclSize capacity, MclSize elemBytes) {
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
for (MclArrayIndex INDEX = 0; INDEX < (ARRAY)->capacity; INDEX++)

#define MCL_ARRAY(CAPACITY, ELEM_BYTES, BUFF) \
{.capacity = (CAPACITY), .elemBytes = (ELEM_BYTES), .buff = (uint8_t*)(BUFF)}

MCL_STDC_END

#endif
