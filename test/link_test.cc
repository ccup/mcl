#include <cctest/cctest.h>
#include "mcl/link.h"

namespace
{
	size_t g_foo_count = 0;

    struct Foo{
		Foo(int x) : x(x) {
		}
		int x;
	};

    Foo* Foo_Create(int x = 0) {
    	Foo *foo = new Foo(x);
    	g_foo_count++;
    	return foo;
    }

    void Foo_Delete(Foo* foo) {
    	delete foo;
    	g_foo_count--;
    }

    void Foo_Sum(const Foo *foo, int *sum) {
    	(*sum) += foo->x;
    }
}

FIXTURE(LinkTest)
{
	MclLink *link;

	BEFORE {
		link = MclLink_Create();
	}

	AFTER {
		MclLink_Delete(link, (MclLinkNodeHandleDeleter)Foo_Delete);
		ASSERT_EQ(0, g_foo_count);
	}

	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetSize(link));
		ASSERT_EQ(NULL, MclLink_FindNodeByHandle(link, 0));
	}

	TEST("should add element to link")
	{
		auto foo = Foo_Create();
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo);
		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetSize(link));

		auto result = MclLink_FindNodeByHandle(link, (uintptr_t)foo);
		ASSERT_EQ((uintptr_t)foo, result->handle);

		ASSERT_EQ(NULL, MclLink_FindNodeByHandle(link, 0));
	}

	TEST("should add more elements to link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo1);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo2);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo3);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(3, MclLink_GetSize(link));

		auto result = MclLink_FindNodeByHandle(link, (uintptr_t)foo1);
		ASSERT_EQ((uintptr_t)foo1, result->handle);

		result = MclLink_FindNodeByHandle(link, (uintptr_t)foo2);
		ASSERT_EQ((uintptr_t)foo2, result->handle);

		result = MclLink_FindNodeByHandle(link, (uintptr_t)foo3);
		ASSERT_EQ((uintptr_t)foo3, result->handle);
	}

	TEST("should remove element from link")
	{
		auto foo = Foo_Create();

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo);
		MclLink_RemoveNodeWithHandle(link, (uintptr_t)foo);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetSize(link));
		ASSERT_EQ(NULL, MclLink_FindNodeByHandle(link, (uintptr_t)foo));

		Foo_Delete(foo);
	}

	TEST("should remove elements from link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo1);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo2);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo3);

		MclLink_RemoveNodeWithHandle(link, (uintptr_t)foo2);
		Foo_Delete(foo2);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(2, MclLink_GetSize(link));

		MclLink_RemoveNodeWithHandle(link, (uintptr_t)foo1);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetSize(link));

		MclLink_RemoveNodeWithHandle(link, (uintptr_t)foo3);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetSize(link));

		Foo_Delete(foo1);
		Foo_Delete(foo3);
	}

	TEST("should delete element from link")
	{
		auto foo = Foo_Create();

		MclLink_AddTailWithHandle(link, (uintptr_t)foo);
		MclLink_DeleteNodeWithHandle(link, (uintptr_t)foo, (MclLinkNodeHandleDeleter)Foo_Delete);

		ASSERT_TRUE(MclLink_IsEmpty(link));
	}

	TEST("should delete elements from link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo1);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo2);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo3);

		MclLink_DeleteNodeWithHandle(link, (uintptr_t)foo3, (MclLinkNodeHandleDeleter)Foo_Delete);
		MclLink_DeleteNodeWithHandle(link, (uintptr_t)foo2, (MclLinkNodeHandleDeleter)Foo_Delete);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetSize(link));

		MclLink_DeleteNodeWithHandle(link, (uintptr_t)foo1, (MclLinkNodeHandleDeleter)Foo_Delete);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetSize(link));
	}

	TEST("should clear element from link")
	{
		auto foo = Foo_Create();

		MclLink_AddTailWithHandle(link, (uintptr_t)foo);

		MclLink_Clear(link,  (MclLinkNodeHandleDeleter)Foo_Delete);

		ASSERT_TRUE(MclLink_IsEmpty(link));
	}

	TEST("should travel each node on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddTailWithHandle(link, (uintptr_t)foo1);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo2);
		MclLink_AddTailWithHandle(link, (uintptr_t)foo3);

		int sum = 0;

		MclLinkNode *node = NULL;
		MCL_LINK_FOR_EACH(link, node) {
			sum += ((Foo*)node->handle)->x;
		}

		ASSERT_EQ(6, sum);

		MclLink_Clear(link, (MclLinkNodeHandleDeleter)Foo_Delete);
	}

	TEST("should travel each node safe on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo1);
		MclLink_AddTailWithHandle(link, (uintptr_t)foo2);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo3);

		int sum = 0;

		MclLinkNode *node = NULL;
		MclLinkNode *tmpNode = NULL;
		MCL_LINK_FOR_EACH_SAFE(link, node, tmpNode) {
			if (node->handle == (uintptr_t)foo2) {
				MclLink_DeleteNode(link, node, (MclLinkNodeHandleDeleter)Foo_Delete);
				continue;
			}
			sum += ((Foo*)node->handle)->x;
		}

		ASSERT_EQ(4, sum);

		MclLink_Clear(link, (MclLinkNodeHandleDeleter)Foo_Delete);
	}

	TEST("should visit each node on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_AddHeadWithHandle(link, (uintptr_t)foo1);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo2);
		MclLink_AddHeadWithHandle(link, (uintptr_t)foo3);

		int sum = 0;

		MCL_LINK_VISIT_VOID(link, Foo, Foo_Sum, &sum);
		ASSERT_EQ(6, sum);

		MclLink_Clear(link, (MclLinkNodeHandleDeleter)Foo_Delete);
	}
};
