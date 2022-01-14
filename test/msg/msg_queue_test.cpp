#include <cctest/cctest.h>
#include "mcl/msg/msg_queue.h"

FIXTURE(MsgQueueTest) {
    constexpr static MclSize MSG_QUEUE_CAPACITY = 10;
    constexpr static MclMsgType defaultType = 3;
    constexpr static MclMsgId defaultId = 2;

    MclMsgQueue *mq {nullptr};

    BEFORE {
        mq = MclMsgQueue_Create(MSG_QUEUE_CAPACITY);
    };

    AFTER {
        MclMsgQueue_Delete(mq);
    };

    TEST("should create an empty mq") {
        ASSERT_TRUE(mq != NULL);
        ASSERT_TRUE(MclMsgQueue_IsEmpty(mq));
        ASSERT_FALSE(MclMsgQueue_IsFull(mq));
        ASSERT_EQ(0, MclMsgQueue_GetCount(mq));
    }

    TEST("should not recv from empty mq") {
        MclMsg result;
        ASSERT_TRUE(MCL_FAILED(MclMsgQueue_Recv(mq, &result)));
    }

    TEST("should send and recv msg") {
        MCL_AUTO_MSG MclMsg *msg = MclMsg_Create(defaultType, defaultId, sizeof(uint64_t));

        uint64_t value = 0xdeadc0de;
        ASSERT_EQ(MCL_SUCCESS, MclMsg_Fill(msg, 0, sizeof(uint64_t), &value));
        ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Send(mq, msg));

        ASSERT_EQ(1, MclMsgQueue_GetCount(mq));
        ASSERT_FALSE(MclMsgQueue_IsEmpty(mq));
        ASSERT_FALSE(MclMsgQueue_IsFull(mq));

        MCL_AUTO_MSG MclMsg *result = MclMsg_Create(defaultType, defaultId, sizeof(uint64_t));
        ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Recv(mq, result));

        ASSERT_EQ(0, MclMsgQueue_GetCount(mq));
        ASSERT_TRUE(MclMsgQueue_IsEmpty(mq));
        ASSERT_FALSE(MclMsgQueue_IsFull(mq));

        ASSERT_EQ(defaultType, result->type);
        ASSERT_EQ(defaultId, result->id);
        ASSERT_EQ(sizeof(uint64_t), result->bodyBytes);

        uint64_t outValue = 0;
        ASSERT_EQ(MCL_SUCCESS, MclMsg_Fetch(result, 0, sizeof(uint64_t), &outValue));
        ASSERT_EQ(value, outValue);
    }

    TEST("should not send to full mq") {
        uint64_t value = 0xdeadc0de;
        MclMsg msg = MCL_MSG(defaultType, defaultId, sizeof(value), &value);

        for (MclSize i = 0; i < MSG_QUEUE_CAPACITY; i++) {
            ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Send(mq, &msg));
            ASSERT_EQ(i + 1, MclMsgQueue_GetCount(mq));
        }
        ASSERT_EQ(MSG_QUEUE_CAPACITY, MclMsgQueue_GetCount(mq));

        ASSERT_TRUE(MCL_FAILED(MclMsgQueue_Send(mq, &msg)));
        ASSERT_EQ(MSG_QUEUE_CAPACITY, MclMsgQueue_GetCount(mq));

        uint64_t outValue = 0;
        MclMsg result = MCL_MSG(0, 0, sizeof(outValue), &outValue);
        ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Recv(mq, &result));
        ASSERT_EQ(MSG_QUEUE_CAPACITY - 1, MclMsgQueue_GetCount(mq));
        ASSERT_FALSE(MclMsgQueue_IsFull(mq));

        ASSERT_EQ(defaultType, result.type);
        ASSERT_EQ(defaultId, result.id);
        ASSERT_EQ(sizeof(uint64_t), result.bodyBytes);
        ASSERT_EQ(value, outValue);
    }

    TEST("should recv from full mq to empty") {
        uint16_t value = 0xabcd;
        MCL_AUTO_MSG MclMsg *msg = MclMsg_Create(defaultType, defaultId, sizeof(value));
        for (MclSize i = 0; i < MSG_QUEUE_CAPACITY; i++) {
            msg->id = i;
            MclMsg_Fill(msg, 0, sizeof(value), &value);
            ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Send(mq, msg));
        }

        ASSERT_EQ(MSG_QUEUE_CAPACITY, MclMsgQueue_GetCount(mq));

        uint16_t outValue = 0;
        MclMsg result = MCL_MSG(0, 0, sizeof(outValue), &outValue);
        for (MclSize i = 0; i < MSG_QUEUE_CAPACITY; i++) {
            ASSERT_EQ(MSG_QUEUE_CAPACITY - i, MclMsgQueue_GetCount(mq));
            ASSERT_EQ(MCL_SUCCESS, MclMsgQueue_Recv(mq, &result));
            ASSERT_EQ(defaultType, result.type);
            ASSERT_EQ(i, result.id);
            ASSERT_EQ(0xabcd, value);
        }
        ASSERT_EQ(0, MclMsgQueue_GetCount(mq));
        ASSERT_TRUE(MCL_FAILED(MclMsgQueue_Recv(mq, &result)));
        ASSERT_EQ(0, MclMsgQueue_GetCount(mq));
    }
};
