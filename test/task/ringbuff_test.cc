#include <cctest/cctest.h>
#include "mcl/task/ringbuff.h"
#include "mcl/task/thread.h"

namespace {
    struct Msg {
        int value;
        bool valid;
    };
}

FIXTURE(RingbuffTest) {
    constexpr static uint16_t RING_BUFF_SIZE = 3;
    constexpr static uint16_t ELEM_SIZE = sizeof(void*);

    MclRingBuff *rb {nullptr};

    BEFORE {
        rb = MclRingBuff_Create(RING_BUFF_SIZE, ELEM_SIZE);
    };

    AFTER {
        MclRingBuff_Delete(rb);
    };

    TEST("should create an empty ringbuff") {
        ASSERT_TRUE(rb != NULL);
        ASSERT_TRUE(MclRingBuff_IsEmpty(rb));
        ASSERT_FALSE(MclRingBuff_IsFull(rb));
    }

    TEST("should not pop from empty ringbuff") {
        Msg result;
        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Pop(rb, &result)));
    }

    TEST("should put and pop element") {
        Msg msg{3, true};
        ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
        ASSERT_FALSE(MclRingBuff_IsEmpty(rb));
        ASSERT_FALSE(MclRingBuff_IsFull(rb));

        Msg result;
        ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Pop(rb, &result));
        ASSERT_TRUE(MclRingBuff_IsEmpty(rb));
        ASSERT_FALSE(MclRingBuff_IsFull(rb));
        ASSERT_EQ(3, result.value);
        ASSERT_EQ(true, result.valid);
    }

    TEST("should not put to full ringbuff") {
        Msg msg{3, true};
        for (size_t i = 0; i < RING_BUFF_SIZE; i++) {
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
        }
        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Put(rb, &msg)));
    }

    TEST("should pop from full ringbuff to empty") {
        for (uint16_t i = 0; i < RING_BUFF_SIZE; i++) {
            Msg msg{i, true};
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
        }

        Msg result;
        for (uint16_t i = 0; i < RING_BUFF_SIZE; i++) {
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Pop(rb, &result));
            ASSERT_EQ(i, result.value);
            ASSERT_EQ(true, result.valid);
        }
        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Pop(rb, &result)));
    }
};

namespace {
    constexpr uint16_t MAX_SEND_VALUE = 10000;

    constexpr uint16_t MSG_QUEUE_SIZE = 10;
    Msg msgBuff[MSG_QUEUE_SIZE] = {0};

    MclRingBuff msgQueue = MCL_RINGBUFF(MSG_QUEUE_SIZE, sizeof(Msg), msgBuff);

    void* sendMsg(void *) {
        uint16_t i = 0;
        while(true) {
            Msg msg {i, true};
            if (MCL_FAILED(MclRingBuff_Put(&msgQueue, &msg))) {
                MclThread_Yield();
            } else {
                if (i++ >= MAX_SEND_VALUE) {
                    break;
                }
            }
        }
        return NULL;
    }

    uint16_t LAST_RECEIVED_VALUD = 0;

    void* recvMsg(void *) {
        while(true) {
            Msg msg {0, false};
            if (MCL_FAILED(MclRingBuff_Pop(&msgQueue, &msg))) {
                MclThread_Yield();
            } else {
                if (msg.valid) {
                    if (msg.value != LAST_RECEIVED_VALUD) {
                        break;
                    }
                    if (LAST_RECEIVED_VALUD++ >= MAX_SEND_VALUE) {
                        break;
                    }
                }
            }
        }
        return NULL;
    }
}

FIXTURE(RingbuffAdvanceTest) {
    TEST("should create an empty ringbuff") {
        MclThread producer, consumer;

        MclThread_Create(&producer, NULL, sendMsg, NULL);
        MclThread_Create(&consumer, NULL, recvMsg,  NULL);

        MclThread_Join(producer, NULL);
        MclThread_Join(consumer, NULL);

        ASSERT_EQ(MAX_SEND_VALUE, LAST_RECEIVED_VALUD);

    }
};
