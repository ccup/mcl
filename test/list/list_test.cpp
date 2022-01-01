#include <cctest/cctest.h>
#include "mcl/list/list.h"

namespace {
	using FooId = uint32_t;
	constexpr FooId FOO_ID_INVALID = 0xFFFFFFFF;

	struct Foo {
		static std::atomic<uint16_t> FOO_COUNT;

		Foo (FooId id) : id {id} {
		}

		FooId getId() const {
			return id;
		}
	private:
		FooId id {FOO_ID_INVALID};
	};

	std::atomic<uint16_t> Foo::FOO_COUNT {0};

	Foo* Foo_Create(FooId id = FOO_ID_INVALID) {
		Foo::FOO_COUNT++;
		return new Foo {id};
	}

	void Foo_Delete(Foo *f) {
		Foo::FOO_COUNT--;
		if (f) delete f;
	}

	void Foo_ListDelete(MclListDataDeleter *deleter, MclListData data) {
	    auto f = (Foo*)data;
	    Foo_Delete(f);
	}

	void Foo_Sum(const Foo *foo, uint32_t *sum) {
		(*sum) += foo->getId();
	}
}

FIXTURE(ListTest)
{
	MclList *list {nullptr};
    MclListDataDeleter fooDeleter;

	BEFORE {
		list = MclList_CreateDefault();
		fooDeleter.destroy = Foo_ListDelete;
	}

	AFTER {
		MclList_Delete(list, &fooDeleter);
		ASSERT_EQ(0, Foo::FOO_COUNT.load());
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

		uint32_t sum = 0;

		MclListNode *node = NULL;
		MCL_LIST_FOREACH(list, node) {
			sum += ((Foo*)node->data)->getId();
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

		uint32_t sum = 0;

		MclListNode *node = NULL;
		MclListNode *tmpNode = NULL;
		MCL_LIST_FOREACH_SAFE(list, node, tmpNode) {
			if (node->data == foo2) {
				MclList_RemoveNode(list, node, &fooDeleter);
				continue;
			}
			sum += ((Foo*)node->data)->getId();
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

		uint32_t sum = 0;

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
