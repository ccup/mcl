#include <cctest/cctest.h>
#include "mcl/mem/allocator.h"

namespace {
	typedef struct Object {
		uint32_t age;
		const char* name;
	} Object;

    constexpr uint16_t OBJECT_SIZE = 2;

    MCL_ALLOCATOR_TYPE_DEF(Object, OBJECT_SIZE);

    MCL_ALLOCATOR_TYPE_DEF(char, OBJECT_SIZE);
}

FIXTURE(AllocatorTest) {
	TEST("alloc object from allocator") {
        MCL_ALLOCATOR(Object) allocator;
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
        MCL_ALLOCATOR(char) allocator;
        charAllocator_Init(&allocator);

		char* a = charAllocator_Alloc(&allocator);
		*a = 'a';
		char* b = charAllocator_Alloc(&allocator);
		*b = 'b';
		char* c = charAllocator_Alloc(&allocator);
		ASSERT_EQ(NULL, c);

        charAllocator_Free(&allocator, a);
		c = charAllocator_Alloc(&allocator);
		ASSERT_EQ(a, c);
	}
};

