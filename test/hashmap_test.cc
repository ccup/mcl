#include <cctest/cctest.h>
#include "mcl/map/hash_map.h"

namespace {

	size_t UNRELEASED_FOO_COUNT = 0;

	constexpr uint32_t INVALID_ID = 0xFFFFFFFF;

    struct Foo{
		Foo(uint32_t id) : id(id) {
		}
		uint32_t id;
	};

    Foo* Foo_Create(uint32_t id) {
    	Foo *foo = new Foo(id);
    	UNRELEASED_FOO_COUNT++;
    	return foo;
    }

    void Foo_Delete(Foo* foo) {
    	delete foo;
    	UNRELEASED_FOO_COUNT--;
    }

    void Foo_Sum(const Foo *foo, uint32_t *sum) {
    	(*sum) += foo->id;
    }

    void HashValueDeleter_DeleteFoo(MclHashValueDeleter *deleter, MclHashValue value) {
        auto f = (Foo*)value;
        Foo_Delete(f);
    }

    void HashValueDeleter_Init(MclHashValueDeleter *deleter) {
    	deleter->destroy = HashValueDeleter_DeleteFoo;
    }

    struct HashNodePred {
    	MclHashNodePred pred;
    	uint32_t id;
    };

    bool HashNodePred_IsEqual(MclHashNodePred *pred, const MclHashNode *node) {
    	HashNodePred *self = MCL_TYPE_REDUCT(pred, HashNodePred, pred);
    	auto foo = (Foo*)(node->value);
    	return self->id == foo->id;
    }

    void HashNodePred_Init(HashNodePred *pred) {
    	pred->pred.pred = HashNodePred_IsEqual;
    	pred->id = INVALID_ID;
    }

    struct HashNodeVisitor {
    	MclHashNodeVisitor visitor;
    	uint32_t sum;
    };

    MclStatus HashNodeVisitor_Sum(MclHashNodeVisitor *visitor, MclHashNode *node) {
    	HashNodeVisitor *self = MCL_TYPE_REDUCT(visitor, HashNodeVisitor, visitor);
    	auto foo = (Foo*)(node->value);
    	self->sum += (INVALID_ID == foo->id) ? 0 : foo->id;
    	return MCL_SUCCESS;
    }

    void HashNodeVisitor_Init(HashNodeVisitor *visitor) {
    	visitor->visitor.visit = HashNodeVisitor_Sum;
    	visitor->sum = 0;
    }
}


FIXTURE(HashMapTest) {
    MclHashMap *map {nullptr};
    MclHashValueDeleter fooDeleter;
    HashNodePred fooIdPred;
    HashNodeVisitor fooVisitor;

    BEFORE {
    	map = MclHashMap_CreateDefault();
    	HashValueDeleter_Init(&fooDeleter);
    	HashNodePred_Init(&fooIdPred);
    	HashNodeVisitor_Init(&fooVisitor);
    }

    AFTER {
		MclHashMap_Delete(map, &fooDeleter);
		ASSERT_EQ(0, UNRELEASED_FOO_COUNT);
    }


	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(0, MclHashMap_GetCount(map));
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(map, 0, NULL)));
	}

	TEST("should add element to map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(map, 1, foo);
		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(1, MclHashMap_GetCount(map));

		Foo *f {nullptr};
		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);

		ASSERT_EQ(1, f->id);
	}

	TEST("should add more elements to map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(map, 1, foo1);
		MclHashMap_Set(map, 2, foo2);
		MclHashMap_Set(map, 3, foo3);

		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(3, MclHashMap_GetCount(map));

		Foo *f {nullptr};

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(1, f->id);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 2, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(2, f->id);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 3, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(3, f->id);
	}

	TEST("should remove element from map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(map, 1, foo);

        MclHashMap_Remove(map, 1, NULL);

		ASSERT_TRUE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(0, MclHashMap_GetCount(map));

		Foo *f {nullptr};
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(map, 1, (MclHashValue*)(&f))));

		Foo_Delete(foo);
	}

	TEST("should remove elements from map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(map, 1, foo1);
		MclHashMap_Set(map, 2, foo2);
		MclHashMap_Set(map, 3, foo3);

		MclHashMap_Remove(map, 2, NULL);
		Foo_Delete(foo2);

		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(2, MclHashMap_GetCount(map));

		MclHashMap_Remove(map, 1, NULL);

		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(1, MclHashMap_GetCount(map));

		MclHashMap_Remove(map, 3, NULL);

		ASSERT_TRUE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(0, MclHashMap_GetCount(map));

		Foo_Delete(foo1);
		Foo_Delete(foo3);
	}

	TEST("should delete element from map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(map, 1, foo);
		MclHashMap_Remove(map, 1, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(map));
	}

	TEST("should delete elements from map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(map, 1, foo1);
		MclHashMap_Set(map, 2, foo2);
		MclHashMap_Set(map, 3, foo3);

		MclHashMap_Remove(map, 3, &fooDeleter);
		MclHashMap_Remove(map, 2, &fooDeleter);

		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(1, MclHashMap_GetCount(map));

		MclHashMap_Remove(map, 1, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(0, MclHashMap_GetCount(map));
	}

	TEST("should clear elements in map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(map, 1, foo);

		MclHashMap_Clear(map, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(map));
	}

	TEST("should remove nodes in map by condition")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(map, 1, foo1);
		MclHashMap_Set(map, 2, foo2);
		MclHashMap_Set(map, 3, foo3);

		fooIdPred.id = 2;
		MclHashMap_RemoveBy(map, &fooIdPred.pred, &fooDeleter);

		ASSERT_FALSE(MclHashMap_IsEmpty(map));
		ASSERT_EQ(2, MclHashMap_GetCount(map));

		Foo *f {nullptr};
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(map, 2, (MclHashValue*)(&f))));

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(map, 3, (MclHashValue*)(&f))));

		MclHashMap_Clear(map, &fooDeleter);
	}

	TEST("should visit nodes in map by condition")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);
		auto foo4 = Foo_Create(4);

		MclHashMap_Set(map, 1, foo1);
		MclHashMap_Set(map, 2, foo2);
		MclHashMap_Set(map, 3, foo3);
		MclHashMap_Set(map, INVALID_ID, Foo_Create(INVALID_ID));
		MclHashMap_Set(map, 4, foo4);

		MclHashMap_Accept(map, &fooVisitor.visitor);
		ASSERT_EQ(10, fooVisitor.sum);

		MclHashMap_Clear(map, &fooDeleter);
	}
};
