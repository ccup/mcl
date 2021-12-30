#ifndef MCL_9C4A7089E0AB41BC8795A6C1A51C3BAE
#define MCL_9C4A7089E0AB41BC8795A6C1A51C3BAE

#include "mcl/msg/msg.h"
#include "mcl/ringbuff/ringbuff.h"
#include "mcl/lock/mutex.h"

MCL_STDC_BEGIN

MCL_TYPE(MclMsgQueue) {
    MclRingBuff queue;
    MclMutex mutex;
    MclMsg *msgBuff;
};

MclMsgQueue* MclMsgQueue_Create(uint16_t capacity);
void MclMsgQueue_Delete(MclMsgQueue*);

MclStatus MclMsgQueue_Init(MclMsgQueue*, uint16_t capacity, MclMsg* msgBuff);
void MclMsgQueue_Clear(MclMsgQueue*);

bool MclMsgQueue_IsFull(const MclMsgQueue*);
bool MclMsgQueue_IsEmpty(const MclMsgQueue*);

uint16_t MclMsgQueue_GetCount(const MclMsgQueue*);

MclStatus MclMsgQueue_Send(MclMsgQueue*, MclMsg*);
MclStatus MclMsgQueue_Recv(MclMsgQueue*, MclMsg*);

MCL_STDC_END

#endif
