#ifndef MCL_9C4A7089E0AB41BC8795A6C1A51C3BAE
#define MCL_9C4A7089E0AB41BC8795A6C1A51C3BAE

#include "mcl/msg/msg.h"
#include "mcl/ringbuff/ringbuff.h"
#include "mcl/lock/mutex.h"

MCL_STDC_BEGIN

MCL_TYPE(MclMsgQueue) {
    MclRingBuff ringbuff;
    MclMutex mutex;
};

MclMsgQueue* MclMsgQueue_Create(MclSize capacity);
void MclMsgQueue_Delete(MclMsgQueue*);

MclStatus MclMsgQueue_Init(MclMsgQueue*, MclSize capacity, MclMsg* msgBuff);
void MclMsgQueue_Destroy(MclMsgQueue*);

void MclMsgQueue_Clear(MclMsgQueue*);

bool MclMsgQueue_IsFull(const MclMsgQueue*);
bool MclMsgQueue_IsEmpty(const MclMsgQueue*);

MclSize MclMsgQueue_GetCount(const MclMsgQueue*);

MclStatus MclMsgQueue_Send(MclMsgQueue*, MclMsg*);
MclStatus MclMsgQueue_Recv(MclMsgQueue*, MclMsg*);

///////////////////////////////////////////////////////////
#define MCL_MSG_QUEUE(MSG_BUFF, CAPACITY) \
{.ringbuff = MCL_RINGBUFF(CAPACITY, sizeof(MclMsg), MSG_BUFF), .mutex = MCL_MUTEX()}

MCL_STDC_END

#endif
