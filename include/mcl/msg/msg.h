#ifndef MCL_0D7A2571016A48AEA8C02B9FFE7452F7
#define MCL_0D7A2571016A48AEA8C02B9FFE7452F7

#include "mcl/msg/msg_priority.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MsgId;

MCL_TYPE(MclMsg) {
    MclMsgPriority priority;
    MsgId id;
    uint32_t rev;
    uint16_t bodyBytes;
    uint8_t body[];
};

MclMsg* MclMsg_Create(uint16_t bodySize);
void MclMsg_Delete(MclMsg*);

MCL_INLINE void MclMsg_Init(MclMsg *self, MclMsgPriority priority, MsgId id, uint32_t rev) {
    if (!self) return;
    self->id = id;
    self->priority = priority;
    self->rev = rev;
}

MCL_INLINE uint16_t MclMsg_GetSize(const MclMsg *self) {
    return self ? sizeof(MclMsg) + self->bodyBytes : 0;
}

MCL_STDC_END

#endif
