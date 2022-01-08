#include <cctest/cctest.h>
#include "mcl/list/list.h"
#include "mcl/mem/allocator.h"

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

    constexpr static uint32_t NODE_NUM_MAX = 6;

    MCL_ALLOCATOR_TYPE_DEF(MclListNodeStaticAllocator, MclListNode, sizeof(MclListNode), NODE_NUM_MAX);

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
        ASSERT_EQ(2, MclList_RemovePredAll(&list, &isLargerThan.pred, NULL));

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
        MclList_PushBackNode(&list, &nodes[1]);
        MclList_PushBackNode(&list, &nodes[3]);
        MclList_PushBackNode(&list, &invalidNode);
        MclList_PushBackNode(&list, &nodes[5]);

        auto sumVisitor = DataVisitor_Create();
		ASSERT_EQ(MCL_SUCCESS, MclList_Accept(&list, &sumVisitor.visitor));
		ASSERT_EQ(4, sumVisitor.sum);
	}
};
