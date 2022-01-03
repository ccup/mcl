#include <cctest/cctest.h>
#include "mcl/algo/bit.h"

FIXTURE(BitTest) {
	TEST ("bit offset") {
		ASSERT_EQ(1, MCL_BIT_OFFSET(uint32_t, 0));
		ASSERT_EQ(2, MCL_BIT_OFFSET(uint32_t, 1));
		ASSERT_EQ(4, MCL_BIT_OFFSET(uint32_t, 2));
		ASSERT_EQ(8, MCL_BIT_OFFSET(uint32_t, 3));
		ASSERT_EQ(16, MCL_BIT_OFFSET(uint32_t, 4));
		ASSERT_EQ(0x80000000, MCL_BIT_OFFSET(uint32_t, 31));

		ASSERT_EQ(0x100000000, MCL_BIT_OFFSET(uint64_t, 32));
		ASSERT_EQ(0x200000000, MCL_BIT_OFFSET(uint64_t, 33));
		ASSERT_EQ(0x8000000000000000, MCL_BIT_OFFSET(uint64_t, 63));
	}

	TEST ("32 bit operations") {
		uint32_t value = 0x80000000;

		ASSERT_FALSE(MCL_BIT_IS_ON_64(value, 0));
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 31));

		MCL_BIT_SET(value, 0);
		ASSERT_TRUE(MCL_BIT_IS_ON(value, 0));
		ASSERT_EQ(0x80000001, value);
		MCL_BIT_CLR(value, 0);
		ASSERT_EQ(0x80000000, value);

		MCL_BIT_SET(value, 1);
		ASSERT_TRUE(MCL_BIT_IS_ON(value, 1));
		ASSERT_EQ(0x80000002, value);
		MCL_BIT_CLR(value, 1);
		ASSERT_EQ(0x80000000, value);

		MCL_BIT_SET(value, 3);
		ASSERT_TRUE(MCL_BIT_IS_ON(value, 3));
		ASSERT_EQ(0x80000008, value);
		MCL_BIT_CLR(value, 3);
		ASSERT_EQ(0x80000000, value);

		MCL_BIT_SET(value, 24);
		ASSERT_TRUE(MCL_BIT_IS_ON(value, 24));
		ASSERT_EQ(0x81000000, value);
		MCL_BIT_CLR(value, 24);
		ASSERT_EQ(0x80000000, value);
	}

	TEST ("64 bit operations") {
		uint64_t value = 0x8000000000000000;

		ASSERT_FALSE(MCL_BIT_IS_ON_64(value, 32));
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 63));

		MCL_BIT_SET_64(value, 0);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 0));
		ASSERT_EQ(0x8000000000000001, value);
		MCL_BIT_CLR_64(value, 0);
		ASSERT_EQ(0x8000000000000000, value);

		MCL_BIT_SET_64(value, 1);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 1));
		ASSERT_EQ(0x8000000000000002, value);
		MCL_BIT_CLR_64(value, 1);
		ASSERT_EQ(0x8000000000000000, value);

		MCL_BIT_SET_64(value, 3);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 3));
		ASSERT_EQ(0x8000000000000008, value);
		MCL_BIT_CLR_64(value, 3);
		ASSERT_EQ(0x8000000000000000, value);

		MCL_BIT_SET_64(value, 24);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 24));
		ASSERT_EQ(0x8000000001000000, value);
		MCL_BIT_CLR_64(value, 24);
		ASSERT_EQ(0x8000000000000000, value);

		MCL_BIT_SET_64(value, 32);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 32));
		ASSERT_EQ(0x8000000100000000, value);
		MCL_BIT_CLR_64(value, 32);
		ASSERT_EQ(0x8000000000000000, value);

		MCL_BIT_SET_64(value, 56);
		ASSERT_TRUE(MCL_BIT_IS_ON_64(value, 56));
		ASSERT_EQ(0x8100000000000000, value);
		MCL_BIT_CLR_64(value, 56);
		ASSERT_EQ(0x8000000000000000, value);
	}

	TEST ("bits set operations") {
		uint32_t value = 0xFF000000;
		uint32_t mask = 0x0000FF00;

		MCL_BITS_SET(value, mask);
		ASSERT_EQ(0xFF00FF00, value);
		ASSERT_TRUE(MCL_BITS_IS_ON(value, mask));
		MCL_BITS_CLR(value, mask);
		ASSERT_EQ(0xFF000000, value);

		MCL_BITS_NEG(value, 0x00F00000);
		ASSERT_EQ(0xFFF00000, value);

		value = 0xFF000000;
		MCL_BITS_ASSIGN(value, mask, 0x01000110);
		ASSERT_EQ(0xFF000100, value);

	}

	TEST ("bits test operations") {
		uint32_t value = 0xFF000000;
		ASSERT_TRUE(MCL_BITS_IS_ON(value, value));
		ASSERT_TRUE(MCL_BITS_IS_OFF(value, 0x00FFFFFF));

		ASSERT_TRUE(MCL_BITS_TEST(value, 0xF0000000));
		ASSERT_TRUE(MCL_BITS_TEST(value, 0x0FF00000));
		ASSERT_FALSE(MCL_BITS_TEST(value, 0x00F00000));
	}
};
