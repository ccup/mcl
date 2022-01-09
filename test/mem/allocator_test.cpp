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

		MCL_ALLOCATOR_INIT(ObjectAllocator, allocator);

		Object* obj1 = MCL_ALLOCATOR_ALLOC(ObjectAllocator, allocator);
		ASSERT_NE(0, obj1);

		Object* obj2 = MCL_ALLOCATOR_ALLOC(ObjectAllocator, allocator);
		ASSERT_NE(0, obj2);

		Object* obj3 = MCL_ALLOCATOR_ALLOC(ObjectAllocator, allocator);
		ASSERT_EQ(NULL, obj3);

        MCL_ALLOCATOR_FREE(ObjectAllocator, allocator, obj1);

        obj3 = MCL_ALLOCATOR_ALLOC(ObjectAllocator, allocator);
		ASSERT_EQ(obj1, obj3);
	}

	TEST("alloc char from allocator") {
        CharAllocator allocator;
        MCL_ALLOCATOR_INIT(CharAllocator, allocator);

		char* a = MCL_ALLOCATOR_ALLOC(CharAllocator, allocator);
		*a = 'a';
		char* b = MCL_ALLOCATOR_ALLOC(CharAllocator, allocator);
		*b = 'b';
		char* c = MCL_ALLOCATOR_ALLOC(CharAllocator, allocator);
		ASSERT_EQ(NULL, c);

		MCL_ALLOCATOR_FREE(CharAllocator, allocator, a);
		c = MCL_ALLOCATOR_ALLOC(CharAllocator, allocator);
		ASSERT_EQ(a, c);
	}
};

