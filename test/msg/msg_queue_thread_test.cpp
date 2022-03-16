#include <cctest/cctest.h>
#include "mcl/msg/msg_queue.h"
#include "mcl/lock/atomic.h"
#include "mcl/thread/thread.h"

namespace {
    constexpr MclSize MSG_QUEUE_CAPACITY = 10;

    MclMsg msgBuff[MSG_QUEUE_CAPACITY];
    MclMsgQueue mq = MCL_MSG_QUEUE(msgBuff, MSG_QUEUE_CAPACITY);

    constexpr uint16_t TRY_COUNT = 10000;
    MclAtomic SENT_COUNT = 0;
    MclAtomic RECV_COUNT = 0;

    uint16_t value = 0xcd;
    uint16_t outValue = 0;

    void* sendMsg(void *) {
        for (uint16_t i = 0; i < TRY_COUNT; i++) {
            MclMsg msg = MCL_MSG(0, i, sizeof(value), &value);
            if (!MCL_FAILED(MclMsgQueue_Send(&mq, &msg))) {
                MclAtomic_AddFetch(&SENT_COUNT, 1);
            }
            MclThread_Yield();
        }
        return NULL;
    }

    void* recvMsg(void *) {
        for (uint16_t i = 0; i < TRY_COUNT; i++) {
            MclMsg msg = MCL_MSG(0, 0, sizeof(outValue), &outValue);
            if (!MCL_FAILED(MclMsgQueue_Recv(&mq, &msg))) {
                MclAtomic_AddFetch(&RECV_COUNT, 1);
            }
            MclThread_Yield();
        }
        return NULL;
    }
}

FIXTURE(MsgQueueThreadTest) {
    BEFORE {
        MclAtomic_Clear(&SENT_COUNT);
        MclAtomic_Clear(&RECV_COUNT);
    }

    TEST("should execute correct in multi threads") {
        MclThread s1, s2, r1, r2;

        MclThread_Create(&s1, NULL, sendMsg, NULL);
        MclThread_Create(&s2, NULL, sendMsg, NULL);
        MclThread_Create(&r1, NULL, recvMsg, NULL);
        MclThread_Create(&r2, NULL, recvMsg, NULL);

        MclThread_Join(s1, NULL);
        MclThread_Join(s2, NULL);
        MclThread_Join(r1, NULL);
        MclThread_Join(r2, NULL);

        ASSERT_EQ(SENT_COUNT, RECV_COUNT + MclMsgQueue_GetCount(&mq));
    }
};
