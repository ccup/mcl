#include <cctest/cctest.h>
#include "mcl/mem/allocator.h"

namespace {
	typedef struct Object {
		uint32_t age;
		const char* name;
	} Object;

	MCL_ALLOCATOR_DEF(Object, 2);

	MCL_ALLOCATOR_DEF(char, 2);
}

FIXTURE(AllocatorTest) {
	TEST("alloc object from allocator") {
		Object* obj1 = MCL_ALLOCATOR(Object).alloc();
		ASSERT_NE(0, obj1);
		Object* obj2 = MCL_ALLOCATOR(Object).alloc();
		ASSERT_NE(0, obj2);
		Object* obj3 = MCL_ALLOCATOR(Object).alloc();
		ASSERT_EQ(NULL, obj3);
		MCL_ALLOCATOR(Object).free(obj1);
		obj3 = MCL_ALLOCATOR(Object).alloc();
		ASSERT_EQ(obj1, obj3);
	}

	TEST("alloc char from allocator") {
		char* a = MCL_ALLOCATOR(char).alloc();
		*a = 'a';
		char* b = MCL_ALLOCATOR(char).alloc();
		*b = 'b';
		char* c = MCL_ALLOCATOR(char).alloc();
		ASSERT_EQ(NULL, c);
		MCL_ALLOCATOR(char).free(a);
		c = MCL_ALLOCATOR(char).alloc();
		ASSERT_EQ(a, c);
	}
};

