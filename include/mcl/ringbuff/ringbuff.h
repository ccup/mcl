#ifndef MCL_BF3E52E326AC45BD894114F46228E796
#define MCL_BF3E52E326AC45BD894114F46228E796

#include "mcl/array/array.h"
#include "mcl/lock/atom.h"

MCL_STDC_BEGIN

MCL_TYPE(MclRingBuff) {
    MclArray buff;
    MclAtom head;
    MclAtom tail;
};

MclRingBuff* MclRingBuff_Create(MclSize capacity, MclSize elemBytes);
void MclRingBuff_Delete(MclRingBuff*);

MclStatus MclRingBuff_Init(MclRingBuff*, MclSize capacity, MclSize elemBytes, uint8_t* buff);
void MclRingBuff_Reset(MclRingBuff*);

bool MclRingBuff_IsFull(const MclRingBuff*);
bool MclRingBuff_IsEmpty(const MclRingBuff*);

MclSize MclRingBuff_GetCount(const MclRingBuff*);

MclStatus MclRingBuff_Pop(MclRingBuff*, void*);
MclStatus MclRingBuff_Put(MclRingBuff*, void*);

///////////////////////////////////////////////////////////
MCL_INLINE void* MclRingBuff_GetBuff(MclRingBuff *self) {
    return self ? MclArray_Begin(&self->buff): NULL;
}

///////////////////////////////////////////////////////////
#define MCL_RINGBUFF(CAPACITY, ELEM_BYTES, BUFF)                   \
{.buff = MCL_ARRAY(CAPACITY, ELEM_BYTES, BUFF), .head = 0, .tail = 0}

MCL_STDC_END

#endif
