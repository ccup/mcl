#ifndef MCL_1E211F5B24D243D88A181C87F5E0ED0B
#define MCL_1E211F5B24D243D88A181C87F5E0ED0B

#include "mcl/array/array.h"

MCL_STDC_BEGIN

MCL_TYPE(MclLinkArray) {
    MclArray array;
    uint16_t freeHead;
    uint16_t freeTail;
    uint16_t freeCount;
};

MclLinkArray* MclLinkArray_Create(uint16_t count, uint16_t elemBytes);
void MclLinkArray_Delete(MclLinkArray*);

MclStatus MclLinkArray_Init(MclLinkArray*, uint16_t count, uint16_t elemBytes, uint8_t* buff);
void MclLinkArray_Clear(MclLinkArray*);

void* MclLinkArray_Take(MclLinkArray*);
void MclLinkArray_Give(MclLinkArray*, void*);

MCL_STDC_END

#endif
