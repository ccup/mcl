#include <cctest/cctest.h>
#include "mcl/list/list.h"

namespace {
	using FooId = uint32_t;
	constexpr FooId FOO_ID_INVALID = MCL_UINT32_MAX;

	struct Foo {
		static MclSize FOO_COUNT;

		Foo (FooId id) : id {id} {
		}

		FooId getId() const {
			return id;
		}
	private:
		FooId id {FOO_ID_INVALID};
	};

	MclSize Foo::FOO_COUNT {0};

	Foo* Foo_Create(FooId id = FOO_ID_INVALID) {
		Foo::FOO_COUNT++;
		return new Foo {id};
	}

	void Foo_Delete(Foo *f) {
		Foo::FOO_COUNT--;
		if (f) delete f;
	}

	void Foo_Sum(const Foo *foo, uint32_t *sum) {
		(*sum) += foo->getId();
	}

	bool FooIdEqualPred_IsEqual(MclListData data, void *arg) {
		auto f = (Foo*)data;
		return f->getId() == (long)arg;
	}
}

FIXTURE(ListTest)
{
	MclList *list {nullptr};

	BEFORE {
		list = MclList_CreateDefault();
	}

	AFTER {
		MclList_Delete(list, (MclListDataDestroy)Foo_Delete);
		ASSERT_EQ(0, Foo::FOO_COUNT);
	}

	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetSize(list));
		ASSERT_EQ(NULL, MclList_FindNode(list, 0));
	}

	TEST("should add element to list")
	{
		auto foo = Foo_Create();
		MclList_PushFront(list, foo);
		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetSize(list));

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
		ASSERT_EQ(3, MclList_GetSize(list));

		auto result = MclList_FindNode(list, foo1);
		ASSERT_EQ(foo1, result->data);

		result = MclList_FindNode(list, foo2);
		ASSERT_EQ(foo2, result->data);

		result = MclList_FindNode(list, foo3);
		ASSERT_EQ(foo3, result->data);
	}

	TEST("should find element in list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

		auto result = (Foo*)MclList_FindByPred(list, FooIdEqualPred_IsEqual, (MclListData)2);
		ASSERT_TRUE(result != NULL);
		ASSERT_EQ(2, result->getId());

		result = (Foo*)MclList_FindByPred(list, FooIdEqualPred_IsEqual, (MclListData)4);
		ASSERT_TRUE(result == NULL);
	}

	TEST("should remove element from list")
	{
		auto foo = Foo_Create();

		MclList_PushFront(list, foo);
        ASSERT_EQ(MCL_SUCCESS, MclList_RemoveData(list, foo, NULL));

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetSize(list));
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

        MclList_RemoveData(list, foo2, (MclListDataDestroy)Foo_Delete);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(2, MclList_GetSize(list));

        MclList_RemoveData(list, foo1, (MclListDataDestroy)Foo_Delete);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetSize(list));

        MclList_RemoveData(list, foo3, (MclListDataDestroy)Foo_Delete);

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetSize(list));
	}

	TEST("should remove satisfied element in list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

		auto result = (Foo*)MclList_RemoveByPred(list, FooIdEqualPred_IsEqual, (MclListData)2);
		ASSERT_EQ(2, MclList_GetSize(list));

		ASSERT_TRUE(result != NULL);
		ASSERT_EQ(2, result->getId());
		Foo_Delete(result);

		result = (Foo*)MclList_RemoveByPred(list, FooIdEqualPred_IsEqual, (MclListData)2);
		ASSERT_EQ(2, MclList_GetSize(list));
		ASSERT_TRUE(result == NULL);

		result = (Foo*)MclList_RemoveByPred(list, FooIdEqualPred_IsEqual, (MclListData)3);
		ASSERT_EQ(1, MclList_GetSize(list));
		ASSERT_TRUE(result != NULL);
		Foo_Delete(result);

		result = (Foo*)MclList_FindByPred(list, FooIdEqualPred_IsEqual, (MclListData)3);
		ASSERT_TRUE(result == NULL);

		result = (Foo*)MclList_FindByPred(list, FooIdEqualPred_IsEqual, (MclListData)1);
		ASSERT_TRUE(result != NULL);
		ASSERT_EQ(1, result->getId());
	}

	TEST("should delete element from list")
	{
		auto foo = Foo_Create();

		MclList_PushBack(list, foo);

        auto result = (Foo*)MclList_RemoveData(list, foo, (MclListDataDestroy)Foo_Delete);

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

        auto result = (Foo*)MclList_RemoveData(list, foo3, (MclListDataDestroy)Foo_Delete);

        result = (Foo*)MclList_RemoveData(list, foo2, (MclListDataDestroy)Foo_Delete);

		ASSERT_FALSE(MclList_IsEmpty(list));
		ASSERT_EQ(1, MclList_GetSize(list));

        result = (Foo*)MclList_RemoveData(list, foo1, (MclListDataDestroy)Foo_Delete);

		ASSERT_TRUE(MclList_IsEmpty(list));
		ASSERT_EQ(0, MclList_GetSize(list));
	}

	TEST("should clear elements in list")
	{
		auto foo = Foo_Create();

		MclList_PushBack(list, foo);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);

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

		uint32_t sum = 0;

		MclListNode *node = NULL;
		MCL_LIST_FOREACH(list, node) {
			sum += ((Foo*)node->data)->getId();
		}

		ASSERT_EQ(6, sum);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);
	}

	TEST("should travel each node safe on list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushBack(list, foo2);
		MclList_PushFront(list, foo3);

		uint32_t sum = 0;

		MclListNode *node = NULL;
		MclListNode *tmpNode = NULL;
		MCL_LIST_FOREACH_SAFE(list, node, tmpNode) {
			if (node->data == foo2) {
				auto f = (Foo*)MclList_RemoveNode(list, node, (MclListDataDestroy)Foo_Delete);
				continue;
			}
			sum += ((Foo*)node->data)->getId();
		}

		ASSERT_EQ(4, sum);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);
	}

	TEST("should visit each node on list")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushFront(list, foo1);
		MclList_PushFront(list, foo2);
		MclList_PushFront(list, foo3);

		uint32_t sum = 0;

		MCL_LIST_FOREACH_CALL(list, Foo, Foo_Sum, &sum);
		ASSERT_EQ(6, sum);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);
	}

	TEST("should insert before node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushBack(list, foo1);
		MclList_PushBack(list, foo3);

		auto node = MclList_FindNode(list, foo3);
		auto newNode = MclList_InsertBefore(list, node, foo2);
		ASSERT_TRUE(newNode != NULL);

		auto f = (Foo*)MclListNode_GetData(newNode);
		ASSERT_EQ(2, f->getId());

		ASSERT_EQ(3, MclList_GetSize(list));

		auto secondNode = MclListNode_GetNext(MclList_GetFirst(list));
		ASSERT_EQ(MclListNode_GetData(secondNode), foo2);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);
	}

	TEST("should insert after node")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclList_PushBack(list, foo1);
		MclList_PushBack(list, foo3);

		auto node = MclList_FindNode(list, foo1);
		auto newNode = MclList_InsertAfter(list, node, foo2);
		ASSERT_TRUE(newNode != NULL);

		auto f = (Foo*)MclListNode_GetData(newNode);
		ASSERT_EQ(2, f->getId());

		ASSERT_EQ(3, MclList_GetSize(list));

		auto secondNode = MclListNode_GetNext(MclList_GetFirst(list));
		ASSERT_EQ(MclListNode_GetData(secondNode), foo2);

		MclList_Clear(list, (MclListDataDestroy)Foo_Delete);
	}
};
