#include <cctest/cctest.h>
#include "mcl/link/link.h"
#include "mcl/link/link_node_allocator.h"

namespace
{
	size_t UNRELEASED_FOO_COUNT = 0;

    struct Foo{
		Foo(int x) : x(x) {
		}
		int x;
	};

    Foo* Foo_Create(int x = 0) {
    	Foo *foo = new Foo(x);
    	UNRELEASED_FOO_COUNT++;
    	return foo;
    }

    void Foo_Delete(Foo* foo) {
    	delete foo;
    	UNRELEASED_FOO_COUNT--;
    }

    void Foo_Sum(const Foo *foo, int *sum) {
    	(*sum) += foo->x;
    }
}

FIXTURE(LinkTest)
{
	MclLink *link;

	BEFORE {
		link = MclLink_Create(MclLinkNodeAllocator_GetDefault());
	}

	AFTER {
		MclLink_Delete(link, (MclLinkDataDeleter)Foo_Delete, NULL);
		ASSERT_EQ(0, UNRELEASED_FOO_COUNT);
	}

	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetCount(link));
		ASSERT_EQ(NULL, MclLink_FindNode(link, 0));
	}

	TEST("should add element to link")
	{
		auto foo = Foo_Create();
		MclLink_PushFront(link, foo);
		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetCount(link));

		auto result = MclLink_FindNode(link, foo);
		ASSERT_EQ(foo, result->data);

		ASSERT_EQ(NULL, MclLink_FindNode(link, 0));
	}

	TEST("should add more elements to link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushFront(link, foo1);
		MclLink_PushFront(link, foo2);
		MclLink_PushFront(link, foo3);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(3, MclLink_GetCount(link));

		auto result = MclLink_FindNode(link, foo1);
		ASSERT_EQ(foo1, result->data);

		result = MclLink_FindNode(link, foo2);
		ASSERT_EQ(foo2, result->data);

		result = MclLink_FindNode(link, foo3);
		ASSERT_EQ(foo3, result->data);
	}

	TEST("should remove element from link")
	{
		auto foo = Foo_Create();

		MclLink_PushFront(link, foo);
        MclLink_RemoveData(link, foo, NULL, NULL);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetCount(link));
		ASSERT_EQ(NULL, MclLink_FindNode(link, foo));

		Foo_Delete(foo);
	}

	TEST("should remove elements from link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushFront(link, foo1);
		MclLink_PushFront(link, foo2);
		MclLink_PushFront(link, foo3);

        MclLink_RemoveData(link, foo2, NULL, NULL);
		Foo_Delete(foo2);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(2, MclLink_GetCount(link));

        MclLink_RemoveData(link, foo1, NULL, NULL);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetCount(link));

        MclLink_RemoveData(link, foo3, NULL, NULL);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetCount(link));

		Foo_Delete(foo1);
		Foo_Delete(foo3);
	}

	TEST("should delete element from link")
	{
		auto foo = Foo_Create();

		MclLink_PushBack(link, foo);
        MclLink_RemoveData(link, foo, (MclLinkDataDeleter)Foo_Delete, NULL);

		ASSERT_TRUE(MclLink_IsEmpty(link));
	}

	TEST("should delete elements from link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushFront(link, foo1);
		MclLink_PushFront(link, foo2);
		MclLink_PushFront(link, foo3);

        MclLink_RemoveData(link, foo3, (MclLinkDataDeleter) Foo_Delete, NULL);
        MclLink_RemoveData(link, foo2, (MclLinkDataDeleter) Foo_Delete, NULL);

		ASSERT_FALSE(MclLink_IsEmpty(link));
		ASSERT_EQ(1, MclLink_GetCount(link));

        MclLink_RemoveData(link, foo1, (MclLinkDataDeleter) Foo_Delete, NULL);

		ASSERT_TRUE(MclLink_IsEmpty(link));
		ASSERT_EQ(0, MclLink_GetCount(link));
	}

	TEST("should clear elements in link")
	{
		auto foo = Foo_Create();

		MclLink_PushBack(link, foo);

		MclLink_Clear(link,  (MclLinkDataDeleter)Foo_Delete, NULL);

		ASSERT_TRUE(MclLink_IsEmpty(link));
	}

	TEST("should travel each node on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushBack(link, foo1);
		MclLink_PushFront(link, foo2);
		MclLink_PushBack(link, foo3);

		int sum = 0;

		MclLinkNode *node = NULL;
		MCL_LINK_FOR_EACH(link, node) {
			sum += ((Foo*)node->data)->x;
		}

		ASSERT_EQ(6, sum);

		MclLink_Clear(link, (MclLinkDataDeleter)Foo_Delete, NULL);
	}

	TEST("should travel each node safe on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushFront(link, foo1);
		MclLink_PushBack(link, foo2);
		MclLink_PushFront(link, foo3);

		int sum = 0;

		MclLinkNode *node = NULL;
		MclLinkNode *tmpNode = NULL;
		MCL_LINK_FOR_EACH_SAFE(link, node, tmpNode) {
			if (node->data == foo2) {
				MclLink_RemoveNode(link, node, (MclLinkDataDeleter)Foo_Delete, NULL);
				continue;
			}
			sum += ((Foo*)node->data)->x;
		}

		ASSERT_EQ(4, sum);

		MclLink_Clear(link, (MclLinkDataDeleter)Foo_Delete, NULL);
	}

	TEST("should visit each node on link")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushFront(link, foo1);
		MclLink_PushFront(link, foo2);
		MclLink_PushFront(link, foo3);

		int sum = 0;

		MCL_LINK_FOR_EACH_CALL(link, Foo, Foo_Sum, &sum);
		ASSERT_EQ(6, sum);

		MclLink_Clear(link, (MclLinkDataDeleter)Foo_Delete, NULL);
	}

	TEST("should insert before node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushBack(link, foo1);
		MclLink_PushBack(link, foo3);

		auto node = MclLink_FindNode(link, foo3);
		ASSERT_EQ(MCL_SUCCESS, MclLink_InsertBefore(link, node, foo2));

		ASSERT_EQ(3, MclLink_GetCount(link));

		auto secondNode = MclLinkNode_GetNext(MclLink_GetFirst(link));
		ASSERT_EQ(MclLinkNode_GetData(secondNode), foo2);

		MclLink_Clear(link, (MclLinkDataDeleter)Foo_Delete, NULL);
	}

	TEST("should insert after node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclLink_PushBack(link, foo1);
		MclLink_PushBack(link, foo3);

		auto node = MclLink_FindNode(link, foo1);
		ASSERT_EQ(MCL_SUCCESS, MclLink_InsertAfter(link, node, foo2));

		ASSERT_EQ(3, MclLink_GetCount(link));

		auto secondNode = MclLinkNode_GetNext(MclLink_GetFirst(link));
		ASSERT_EQ(MclLinkNode_GetData(secondNode), foo2);

		MclLink_Clear(link, (MclLinkDataDeleter)Foo_Delete, NULL);
	}
};

namespace {
	const long INVALID_DATA = 0xFFFFFFFF;

	MclStatus Data_Visit(MclLinkData data, void *arg) {
		long value = (long)data;
		if (value == INVALID_DATA) return MCL_STATUS_DONE;
		(*(long*)arg) += value;
		return MCL_SUCCESS;
	}

	bool Data_IsLargerThan(MclLinkData data, void *arg) {
		return data > arg;
	}
}

FIXTURE(LinkAdvanceTest)
{
    constexpr static uint32_t NODE_NUM = 6;

	MclLink link;
	MclLinkNode nodes[NODE_NUM];

	LinkAdvanceTest() {
		MclLink_Init(&link, NULL);

		for (long i = 0; i < NODE_NUM; i++) {
		    MclLinkNode_Init(&nodes[i], (MclLinkData)i);
		}
	}

	AFTER {
		MclLink_Clear(&link, NULL, NULL);
	}

	TEST("find all valid data")
	{
		MclLink_PushBackNode(&link, &nodes[1]);
		MclLink_PushBackNode(&link, &nodes[3]);
		MclLink_PushBackNode(&link, &nodes[5]);
		MclLink_PushBackNode(&link, &nodes[2]);

        MclLink result = MCL_LINK_INITIALIZE(result, MclLinkNodeAllocator_GetDefault());
//        MclLink_Init(&result, MclLinkNodeAllocator_GetDefault());
		MclLink_FindBy(&link, Data_IsLargerThan, (MclLinkData)2, &result);

		ASSERT_EQ(2, MclLink_GetCount(&result));

		auto firstNode = MclLink_GetFirst(&result);
		ASSERT_EQ(3, (long)MclLinkNode_GetData(firstNode));

		auto secondNode = MclLinkNode_GetNext(firstNode);
		ASSERT_EQ(5, (long)MclLinkNode_GetData(secondNode));

        MclLink_Clear(&result, NULL, NULL);
	}

	TEST("should remove all matched nodes in link")
    {
        MclLink_PushBackNode(&link, &nodes[1]);
        MclLink_PushBackNode(&link, &nodes[3]);
        MclLink_PushBackNode(&link, &nodes[5]);
        MclLink_PushBackNode(&link, &nodes[2]);

        MclLink_RemoveBy(&link, Data_IsLargerThan, (MclLinkData)2, NULL, NULL);

        ASSERT_EQ(2, MclLink_GetCount(&link));

        auto firstNode = MclLink_GetFirst(&link);
        ASSERT_EQ(1, (long)MclLinkNode_GetData(firstNode));

        auto secondNode = MclLinkNode_GetNext(firstNode);
        ASSERT_EQ(2, (long)MclLinkNode_GetData(secondNode));
    }

	TEST("should visit all nodes in link")
	{
        MclLink_PushBackNode(&link, &nodes[1]);
        MclLink_PushBackNode(&link, &nodes[3]);
        MclLink_PushBackNode(&link, &nodes[5]);

		long sum = 0;
		ASSERT_EQ(MCL_SUCCESS, MclLink_Accept(&link, Data_Visit, &sum));
		ASSERT_EQ(9, sum);
	}

	TEST("should visit nodes until invalid")
	{
        MclLinkNode invalidNode;
        MclLinkNode_Init(&invalidNode, (MclLinkData)INVALID_DATA);

        MclLink_PushBackNode(&link, &nodes[1]);
        MclLink_PushBackNode(&link, &nodes[3]);
        MclLink_PushBackNode(&link, &invalidNode);
        MclLink_PushBackNode(&link, &nodes[5]);

		long sum = 0;
		ASSERT_EQ(MCL_SUCCESS, MclLink_Accept(&link, Data_Visit, &sum));
		ASSERT_EQ(4, sum);
	}
};
