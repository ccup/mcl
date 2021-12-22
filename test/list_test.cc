#include <cctest/cctest.h>
#include "mcl/list/list.h"
#include "mcl/list/list_node_allocator.h"

namespace {

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

    void ListDataDeleter_DeleteFoo(MclListDataDeleter *deleter, MclListData data) {
        auto f = (Foo*)data;
        Foo_Delete(f);
    }
}

FIXTURE(ListTest)
{
	MclList *list {nullptr};
    MclListDataDeleter fooDeleter;

	BEFORE {
		list = MclList_CreateDefault();
		fooDeleter.destroy = ListDataDeleter_DeleteFoo;
	}

	AFTER {
		MclList_Delete(list, &fooDeleter);
		ASSERT_EQ(0, UNRELEASED_FOO_COUNT);
	}

	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetCount(list));
		ASSERT_EQ(NULL, MclList_FindNode(list, 0));
	}

	TEST("should add element to list")
	{
		auto foo = Foo_Create();
		MclList_PushFront(list, foo);
		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetCount(list));

		auto result = MclList_FindNode(list, foo);
		ASSERT_EQ(foo, result->data);

		ASSERT_EQ(NULL, MclList_FindNode(list, 0));
	}

	TEST("should add more elements to list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(3, MclList_GetCount(list));

		auto result = MclList_FindNode(list, foo1);
		ASSERT_EQ(foo1, result->data);

		result = MclList_FindNode(list, foo2);
		ASSERT_EQ(foo2, result->data);

		result = MclList_FindNode(list, foo3);
		ASSERT_EQ(foo3, result->data);
	}

	TEST("should remove element from list")
	{
		auto foo = Foo_Create();

		MclList_PushFront(list, foo);
        ASSERT_EQ(1, MclList_RemoveData(list, foo, NULL));

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetCount(list));
		ASSERT_EQ(NULL, MclList_FindNode(list, foo));

		Foo_Delete(foo);
	}

	TEST("should remove elements from list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

        MclList_RemoveData(list, foo2, NULL);
		Foo_Delete(foo2);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(2, MclList_GetCount(list));

        MclList_RemoveData(list, foo1, NULL);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetCount(list));

        MclList_RemoveData(list, foo3, NULL);

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetCount(list));

		Foo_Delete(foo1);
		Foo_Delete(foo3);
	}

	TEST("should delete element from list")
	{
		auto foo = Foo_Create();

		MclList_PushBack(list, foo);

        MclList_RemoveData(list, foo, &fooDeleter);

		ASSERT_TRUE(MclList_IsEmpty(list));
	}

	TEST("should delete elements from list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

        MclList_RemoveData(list, foo3, &fooDeleter);
        MclList_RemoveData(list, foo2, &fooDeleter);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetCount(list));

        MclList_RemoveData(list, foo1, &fooDeleter);

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetCount(list));
	}

	TEST("should clear elements in list")
	{
		auto foo = Foo_Create();

		MclList_PushBack(list, foo);

		MclList_Clear(list, &fooDeleter);

		ASSERT_TRUE(MclList_IsEmpty(list));
	}

	TEST("should travel each node on list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushBack(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushBack(list, foo3);

		int sum = 0;

		MclListNode *node = NULL;
		MCL_LIST_FOREACH(list, node) {
			sum += ((Foo*)node->data)->x;
		}

		ASSERT_EQ(6, sum);

		MclList_Clear(list, &fooDeleter);
	}

	TEST("should travel each node safe on list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushBack(list, foo2);
		MclList_PushFront(list, foo3);

		int sum = 0;

		MclListNode *node = NULL;
		MclListNode *tmpNode = NULL;
		MCL_LIST_FOREACH_SAFE(list, node, tmpNode) {
			if (node->data == foo2) {
				MclList_RemoveNode(list, node, &fooDeleter);
				continue;
			}
			sum += ((Foo*)node->data)->x;
		}

		ASSERT_EQ(4, sum);

		MclList_Clear(list, &fooDeleter);
	}

	TEST("should visit each node on list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

		int sum = 0;

		MCL_LIST_FOREACH_CALL(list, Foo, Foo_Sum, &sum);
		ASSERT_EQ(6, sum);

		MclList_Clear(list, &fooDeleter);
	}

	TEST("should insert before node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushBack(list, foo1);
		MclList_PushBack(list, foo3);

		auto node = MclList_FindNode(list, foo3);
		ASSERT_EQ(MCL_SUCCESS, MclList_InsertBefore(list, node, foo2));

		ASSERT_EQ(3, MclList_GetCount(list));

		auto secondNode = MclListNode_GetNext(MclList_GetFirst(list));
		ASSERT_EQ(MclListNode_GetData(secondNode), foo2);

		MclList_Clear(list, &fooDeleter);
	}

	TEST("should insert after node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushBack(list, foo1);
		MclList_PushBack(list, foo3);

		auto node = MclList_FindNode(list, foo1);
		ASSERT_EQ(MCL_SUCCESS, MclList_InsertAfter(list, node, foo2));

		ASSERT_EQ(3, MclList_GetCount(list));

		auto secondNode = MclListNode_GetNext(MclList_GetFirst(list));
		ASSERT_EQ(MclListNode_GetData(secondNode), foo2);

		MclList_Clear(list, &fooDeleter);
	}
};

namespace {
	const long INVALID_DATA = 0xFFFFFFFF;

	struct DataVisitor {
	    MclListDataVisitor visitor;
	    long sum;
	};

    MclStatus DataVisitor_Sum(MclListDataVisitor *visitor, MclListData data) {
        auto v = (long)data;
        if (v == INVALID_DATA) return MCL_STATUS_DONE;
        DataVisitor *self = MCL_TYPE_REDUCT(visitor, DataVisitor, visitor);
        self->sum += v;
        return MCL_SUCCESS;
    }

    DataVisitor DataVisitor_Create() {
        DataVisitor visitor = {.visitor = MCL_LIST_DATA_VISITOR(DataVisitor_Sum), .sum = 0};
        return visitor;
    }

	struct DataPred {
	    MclListDataPred pred;
        long arg;
	};

	bool DataPred_IsLargerThan(MclListDataPred *pred, MclListData data) {
	    auto v = (long)data;
        DataPred *self = MCL_TYPE_REDUCT(pred, DataPred, pred);
        return v > self->arg;
	}

    DataPred DataPred_Create(long arg) {
        DataPred pred = {.pred = MCL_LIST_DATA_PRED(DataPred_IsLargerThan), .arg = arg};
        return pred;
	}
}

FIXTURE(ListAdvanceTest)
{
    constexpr static uint32_t NODE_NUM = 6;

	MclList list;
	MclListNode nodes[NODE_NUM];

	ListAdvanceTest() {
		MclList_Init(&list, NULL);

		for (long i = 0; i < NODE_NUM; i++) {
		    MclListNode_Init(&nodes[i], (MclListData)i);
		}
	}

	AFTER {
		MclList_Clear(&list, NULL);
	}

	TEST("find all valid data")
	{
		MclList_PushBackNode(&list, &nodes[1]);
		MclList_PushBackNode(&list, &nodes[3]);
		MclList_PushBackNode(&list, &nodes[5]);
		MclList_PushBackNode(&list, &nodes[2]);

        MclList result = MCL_LIST(result, MclListNodeAllocator_GetDefault());

        auto isLargerThan = DataPred_Create(2);
		MclList_FindBy(&list, &isLargerThan.pred, &result);

		ASSERT_EQ(2, MclList_GetCount(&result));

		auto firstNode = MclList_GetFirst(&result);
		ASSERT_EQ(3, (long)MclListNode_GetData(firstNode));

		auto secondNode = MclListNode_GetNext(firstNode);
		ASSERT_EQ(5, (long)MclListNode_GetData(secondNode));

        MclList_Clear(&result, NULL);
	}

	TEST("should remove all matched nodes in list")
    {
        MclList_PushBackNode(&list, &nodes[1]);
        MclList_PushBackNode(&list, &nodes[3]);
        MclList_PushBackNode(&list, &nodes[5]);
        MclList_PushBackNode(&list, &nodes[2]);

        auto isLargerThan = DataPred_Create(2);
        ASSERT_EQ(2, MclList_RemoveBy(&list, &isLargerThan.pred, NULL));

        ASSERT_EQ(2, MclList_GetCount(&list));

        auto firstNode = MclList_GetFirst(&list);
        ASSERT_EQ(1, (long)MclListNode_GetData(firstNode));

        auto secondNode = MclListNode_GetNext(firstNode);
        ASSERT_EQ(2, (long)MclListNode_GetData(secondNode));
    }

	TEST("should visit all nodes in list")
	{
        MclList_PushBackNode(&list, &nodes[1]);
        MclList_PushBackNode(&list, &nodes[3]);
        MclList_PushBackNode(&list, &nodes[5]);

        auto sumVisitor = DataVisitor_Create();
		ASSERT_EQ(MCL_SUCCESS, MclList_Accept(&list, &sumVisitor.visitor));
		ASSERT_EQ(9, sumVisitor.sum);
	}

	TEST("should visit nodes until invalid")
	{
        MclListNode invalidNode;
        MclListNode_Init(&invalidNode, (MclListData)INVALID_DATA);

        MclList_PushBackNode(&list, &nodes[1]);
        MclList_PushBackNode(&list, &nodes[3]);
        MclList_PushBackNode(&list, &invalidNode);
        MclList_PushBackNode(&list, &nodes[5]);

        auto sumVisitor = DataVisitor_Create();
		ASSERT_EQ(MCL_SUCCESS, MclList_Accept(&list, &sumVisitor.visitor));
		ASSERT_EQ(4, sumVisitor.sum);
	}
};
