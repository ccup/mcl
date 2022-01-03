#include <cctest/cctest.h>
#include "mcl/mem/allocator.h"

namespace {
	MCL_TYPE(Object) {
		uint32_t age;
		const char* name;
	};

    constexpr uint16_t ALLOCATOR_CAPACITY = 2;

    MCL_ALLOCATOR_TYPE_DEF(ObjectAllocator, Object, ALLOCATOR_CAPACITY);

    MCL_ALLOCATOR_TYPE_DEF(CharAllocator, char, ALLOCATOR_CAPACITY);
}

FIXTURE(AllocatorTest) {
	TEST("alloc object from allocator") {
		ObjectAllocator allocator;
        ObjectAllocator_Init(&allocator);

		Object* obj1 = ObjectAllocator_Alloc(&allocator);
		ASSERT_NE(0, obj1);

		Object* obj2 = ObjectAllocator_Alloc(&allocator);
		ASSERT_NE(0, obj2);

		Object* obj3 = ObjectAllocator_Alloc(&allocator);
		ASSERT_EQ(NULL, obj3);

        ObjectAllocator_Free(&allocator, obj1);

        obj3 = ObjectAllocator_Alloc(&allocator);
		ASSERT_EQ(obj1, obj3);
	}

	TEST("alloc char from allocator") {
        CharAllocator allocator;
        CharAllocator_Init(&allocator);

		char* a = CharAllocator_Alloc(&allocator);
		*a = 'a';
		char* b = CharAllocator_Alloc(&allocator);
		*b = 'b';
		char* c = CharAllocator_Alloc(&allocator);
		ASSERT_EQ(NULL, c);

        CharAllocator_Free(&allocator, a);
		c = CharAllocator_Alloc(&allocator);
		ASSERT_EQ(a, c);
	}
};

