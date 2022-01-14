#include <cctest/cctest.h>
#include "mcl/ringbuff/ringbuff.h"
#include "mcl/thread/thread.h"

namespace {
    struct Msg {
        uint32_t value;
        bool valid;
    };

    constexpr MclSize MAX_SEND_VALUE = 10000;

    constexpr MclSize MSG_QUEUE_SIZE = 10;
    Msg msgBuff[MSG_QUEUE_SIZE] = {0};

    MclRingBuff msgQueue = MCL_RINGBUFF(MSG_QUEUE_SIZE, sizeof(Msg), (uint8_t*)msgBuff);

    void* sendMsg(void *) {
    	MclSize i = 0;
        while(true) {
            Msg msg {i, true};
            if (!MCL_FAILED(MclRingBuff_Put(&msgQueue, &msg))) {
                if (i++ >= MAX_SEND_VALUE) {
                    break;
                }
            }
            MclThread_Yield();
        }
        MCL_LOG_SUCC("Msg queue send thread quit OK!");
        return NULL;
    }

    MclSize LAST_RECEIVED_VALUD = 0;

    void* recvMsg(void *) {
        while(true) {
            Msg msg{0, false};
            if (!MCL_FAILED(MclRingBuff_Pop(&msgQueue, &msg))) {
                if (msg.valid) {
                    if (msg.value != LAST_RECEIVED_VALUD) {
                        MCL_LOG_ERR("Msg queue recv failed!");
                        break;
                    }
                    if (++LAST_RECEIVED_VALUD >= MAX_SEND_VALUE) {
                        break;
                    }
                }
            }
            MclThread_Yield();
        }
        MCL_LOG_SUCC("Msg queue recv thread quit OK!");
        return NULL;
    }
}

FIXTURE(RingbuffAdvanceTest) {
    TEST("should run correctly in producer and consumer threads") {
        MclThread producer, consumer;

        MclThread_Create(&producer, NULL, sendMsg, NULL);
        MclThread_Create(&consumer, NULL, recvMsg,  NULL);

        MclThread_Join(producer, NULL);
        MclThread_Join(consumer, NULL);

        ASSERT_EQ(MAX_SEND_VALUE, LAST_RECEIVED_VALUD);
    }
};
