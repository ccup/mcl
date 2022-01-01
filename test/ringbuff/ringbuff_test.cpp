#include <cctest/cctest.h>
#include "mcl/ringbuff/ringbuff.h"

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
        ASSERT_EQ(0, MclRingBuff_GetCount(rb));
    }

    TEST("should not pop from empty ringbuff") {
        Msg result;
        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Pop(rb, &result)));
    }

    TEST("should put and pop element") {
        Msg msg{3, true};
        ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
        ASSERT_EQ(1, MclRingBuff_GetCount(rb));
        ASSERT_FALSE(MclRingBuff_IsEmpty(rb));
        ASSERT_FALSE(MclRingBuff_IsFull(rb));

        Msg result;
        ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Pop(rb, &result));
        ASSERT_EQ(0, MclRingBuff_GetCount(rb));
        ASSERT_TRUE(MclRingBuff_IsEmpty(rb));
        ASSERT_FALSE(MclRingBuff_IsFull(rb));
        ASSERT_EQ(3, result.value);
        ASSERT_EQ(true, result.valid);
    }

    TEST("should not put to full ringbuff") {
        Msg msg{3, true};
        for (size_t i = 0; i < RING_BUFF_SIZE - 1; i++) {
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
            ASSERT_EQ(i + 1, MclRingBuff_GetCount(rb));
        }
        ASSERT_EQ(RING_BUFF_SIZE - 1, MclRingBuff_GetCount(rb));

        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Put(rb, &msg)));
        ASSERT_EQ(RING_BUFF_SIZE - 1, MclRingBuff_GetCount(rb));
    }

    TEST("should pop from full ringbuff to empty") {
        for (uint16_t i = 0; i < RING_BUFF_SIZE - 1; i++) {
            Msg msg{i, true};
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Put(rb, &msg));
        }

        ASSERT_EQ(RING_BUFF_SIZE - 1, MclRingBuff_GetCount(rb));

        Msg result;
        for (uint16_t i = 0; i < RING_BUFF_SIZE - 1; i++) {
            ASSERT_EQ(RING_BUFF_SIZE - i - 1, MclRingBuff_GetCount(rb));
            ASSERT_EQ(MCL_SUCCESS, MclRingBuff_Pop(rb, &result));
            ASSERT_EQ(i, result.value);
            ASSERT_EQ(true, result.valid);
        }
        ASSERT_EQ(0, MclRingBuff_GetCount(rb));

        ASSERT_TRUE(MCL_FAILED(MclRingBuff_Pop(rb, &result)));
        ASSERT_EQ(0, MclRingBuff_GetCount(rb));
    }
};
