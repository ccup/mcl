#include <cctest/cctest.h>
#include "mcl/list/list.h"
#include "mcl/mem/allocator.h"

namespace {
	const long INVALID_DATA = MCL_UINT32_MAX;

    MclStatus DataVisitor_Sum(MclListData data, void *arg) {
        auto v = (long)data;
        if (v == INVALID_DATA) return MCL_STATUS_DONE;
        long *sum = (long*)arg;
        (*sum) += v;
        return MCL_SUCCESS;
    }

    bool DataPred_IsLargerThan(MclListData data, void *arg) {
    	return (long)data > (long)arg;
    }

    constexpr static uint32_t NODE_NUM_MAX = 6;

    MCL_ALLOCATOR_TYPE_DEF(MclListNodeStaticAllocator, MclListNode, NODE_NUM_MAX);

    MclListNodeStaticAllocator nodeAllocator;

    MclListNode* ListNode_StaticAlloc(MclListNodeAllocator *allocator) {
    	return MCL_ALLOCATOR_ALLOC(MclListNodeStaticAllocator, nodeAllocator);
    }

    void ListNode_StaticFree(MclListNodeAllocator *allocator, MclListNode *node) {
    	MCL_ALLOCATOR_FREE(MclListNodeStaticAllocator, nodeAllocator, node);
    }

    MclList list = MCL_LIST(list, NULL);
}

FIXTURE(ListStaticTest)
{
	MclListNode nodes[NODE_NUM_MAX];
	MclListNode invalidNode;

	ListStaticTest() {
		MCL_ALLOCATOR_INIT(MclListNodeStaticAllocator, nodeAllocator);

		for (long i = 0; i < NODE_NUM_MAX; i++) {
		    MclListNode_Init(&nodes[i], (MclListData)i);
		}
		MclListNode_Init(&invalidNode, (MclListData)INVALID_DATA);
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

		MclListNodeAllocator allocator = MCL_LIST_NODE_ALLOCATOR(ListNode_StaticAlloc, ListNode_StaticFree);
        MclList result = MCL_LIST(result, &allocator);

		MclList_FindAllByPred(&list, DataPred_IsLargerThan, (MclListData)2, &result);

		ASSERT_EQ(2, MclList_GetSize(&result));

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

        ASSERT_EQ(2, MclList_RemoveAllByPred(&list, DataPred_IsLargerThan, (MclListData)2, NULL));

        ASSERT_EQ(2, MclList_GetSize(&list));

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

        long sum = 0;
		ASSERT_EQ(MCL_SUCCESS, MclList_Accept(&list, DataVisitor_Sum, &sum));
		ASSERT_EQ(9, sum);
	}

	TEST("should visit nodes until invalid")
	{
        MclList_PushBackNode(&list, &nodes[1]);
        MclList_PushBackNode(&list, &nodes[3]);
        MclList_PushBackNode(&list, &invalidNode);
        MclList_PushBackNode(&list, &nodes[5]);

        long sum = 0;
		ASSERT_EQ(MCL_SUCCESS, MclList_Accept(&list, DataVisitor_Sum, &sum));
		ASSERT_EQ(4, sum);
	}
};
