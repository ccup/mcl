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
    	if (INVALID_ID == foo->id) return MCL_STATUS_DONE;
    	self->sum += foo->id;
    	return MCL_SUCCESS;
    }

    void HashNodeVisitor_Init(HashNodeVisitor *visitor) {
    	visitor->visitor.visit = HashNodeVisitor_Sum;
    	visitor->sum = 0;
    }
}


FIXTURE(HashMapTest) {
    MclHashMap *foos {nullptr};
    MclHashValueDeleter fooDeleter;
    HashNodePred fooIdPred;
    HashNodeVisitor fooVisitor;

    BEFORE {
    	foos = MclHashMap_CreateDefault();
    	HashValueDeleter_Init(&fooDeleter);
    	HashNodePred_Init(&fooIdPred);
    	HashNodeVisitor_Init(&fooVisitor);
    }

    AFTER {
		MclHashMap_Delete(foos, &fooDeleter);
		ASSERT_EQ(0, UNRELEASED_FOO_COUNT);
    }


	TEST("should be empty when initialized")
	{
		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(0, MclHashMap_GetCount(foos));
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(foos, 0, NULL)));
	}

	TEST("should add element to map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(foos, 1, foo);
		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(1, MclHashMap_GetCount(foos));

		Foo *f {nullptr};
		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);

		ASSERT_EQ(1, f->id);
	}

	TEST("should add more elements to map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(foos, 1, foo1);
		MclHashMap_Set(foos, 2, foo2);
		MclHashMap_Set(foos, 3, foo3);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(3, MclHashMap_GetCount(foos));

		Foo *f {nullptr};

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(1, f->id);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 2, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(2, f->id);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 3, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(3, f->id);
	}

	TEST("should remove element from map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(foos, 1, foo);

        MclHashMap_Remove(foos, 1, NULL);

		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(0, MclHashMap_GetCount(foos));

		Foo *f {nullptr};
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(foos, 1, (MclHashValue*)(&f))));

		Foo_Delete(foo);
	}

	TEST("should remove elements from map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(foos, 1, foo1);
		MclHashMap_Set(foos, 2, foo2);
		MclHashMap_Set(foos, 3, foo3);

		MclHashMap_Remove(foos, 2, NULL);
		Foo_Delete(foo2);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(2, MclHashMap_GetCount(foos));

		MclHashMap_Remove(foos, 1, NULL);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(1, MclHashMap_GetCount(foos));

		MclHashMap_Remove(foos, 3, NULL);

		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(0, MclHashMap_GetCount(foos));

		Foo_Delete(foo1);
		Foo_Delete(foo3);
	}

	TEST("should delete element from map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(foos, 1, foo);
		MclHashMap_Remove(foos, 1, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
	}

	TEST("should delete elements from map")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(foos, 1, foo1);
		MclHashMap_Set(foos, 2, foo2);
		MclHashMap_Set(foos, 3, foo3);

		MclHashMap_Remove(foos, 3, &fooDeleter);
		MclHashMap_Remove(foos, 2, &fooDeleter);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(1, MclHashMap_GetCount(foos));

		MclHashMap_Remove(foos, 1, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(0, MclHashMap_GetCount(foos));
	}

	TEST("should clear elements in map")
	{
		auto foo = Foo_Create(1);

		MclHashMap_Set(foos, 1, foo);

		MclHashMap_Clear(foos, &fooDeleter);

		ASSERT_TRUE(MclHashMap_IsEmpty(foos));
	}

	TEST("should remove nodes in map by condition")
	{
		auto foo1 = Foo_Create(1);
		auto foo2 = Foo_Create(2);
		auto foo3 = Foo_Create(3);

		MclHashMap_Set(foos, 1, foo1);
		MclHashMap_Set(foos, 2, foo2);
		MclHashMap_Set(foos, 3, foo3);

		fooIdPred.id = 2;
		MclHashMap_RemoveBy(foos, &fooIdPred.pred, &fooDeleter);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(2, MclHashMap_GetCount(foos));

		Foo *f {nullptr};
		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(foos, 2, (MclHashValue*)(&f))));

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 1, (MclHashValue*)(&f))));
		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 3, (MclHashValue*)(&f))));

		MclHashMap_Clear(foos, &fooDeleter);
	}

	TEST("should visit nodes in map by condition")
	{
		MclHashMap_Set(foos, 12, Foo_Create(12));
		MclHashMap_Set(foos, 13, Foo_Create(13));
		MclHashMap_Set(foos, 14, Foo_Create(14));
		MclHashMap_Set(foos, INVALID_ID, Foo_Create(INVALID_ID));
		MclHashMap_Set(foos, 15, Foo_Create(15));

		MclHashMap_Accept(foos, &fooVisitor.visitor);
		ASSERT_EQ(39, fooVisitor.sum);

		MclHashMap_Clear(foos, &fooDeleter);
	}

	TEST("should add more elements")
	{
		constexpr uint32_t MAX_ELEMS = 10000;

		for (uint32_t i = 0; i < MAX_ELEMS; i++) {
			MclHashMap_Set(foos, i, Foo_Create(i));
		}

		ASSERT_EQ(MAX_ELEMS, MclHashMap_GetCount(foos));

		for (uint32_t i = 0; i < MAX_ELEMS; i++) {
			Foo *f {nullptr};
			ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, i, (MclHashValue*)(&f))));
			ASSERT_TRUE(f != nullptr);
			ASSERT_EQ(i, f->id);
		}

		for (uint32_t i = 0; i < MAX_ELEMS - 2; i++) {
			MclHashMap_Remove(foos, i, &fooDeleter);
		}

		ASSERT_EQ(2, MclHashMap_GetCount(foos));

		MclHashMap_Accept(foos, &fooVisitor.visitor);
		ASSERT_EQ(MAX_ELEMS + MAX_ELEMS - 3, fooVisitor.sum);

		MclHashMap_Clear(foos, &fooDeleter);
	}
};

namespace {
	struct FooMap {
		MclHashMap map;
		MclHashBucket buckets[MCL_HASHMAP_BUCKET_COUNT_DEFAULT];
	};
}

FIXTURE(HashMapAdvanceTest) {
	constexpr static uint32_t NODE_NUM = 6;

	FooMap foomap;
	MclHashNode nodes[NODE_NUM];

	MclHashMap *foos{nullptr};

    HashMapAdvanceTest() {
    	foos = &foomap.map;
		MclHashMap_Init(foos, MCL_HASHMAP_BUCKET_COUNT_DEFAULT, foomap.buckets, NULL);

		for (long i = 0; i < NODE_NUM; i++) {
		    MclHashNode_Init(&nodes[i], (MclHashKey)i, (MclHashValue)i);
		}
	}

    AFTER {
		MclHashMap_Clear(foos, NULL);
    }

	TEST("find all valid data")
	{
		MclHashMap_InsertNode(foos, &nodes[1]);
		MclHashMap_InsertNode(foos, &nodes[3]);
		MclHashMap_InsertNode(foos, &nodes[5]);
		MclHashMap_InsertNode(foos, &nodes[2]);

		ASSERT_FALSE(MclHashMap_IsEmpty(foos));
		ASSERT_EQ(4, MclHashMap_GetCount(foos));

		long value{0};

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 1, (MclHashValue*)(&value))));
		ASSERT_EQ(1, value);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 2, (MclHashValue*)(&value))));
		ASSERT_EQ(2, value);

		ASSERT_TRUE(MCL_FAILED(MclHashMap_Get(foos, 4, (MclHashValue*)(&value))));

		auto result = MclHashMap_FindNode(foos, 5);
		ASSERT_TRUE(result != NULL);
		ASSERT_EQ(5, result->key);
		ASSERT_EQ(5, (long)result->value);

		ASSERT_TRUE(MclHashMap_FindNode(foos, 6) == NULL);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_RemoveNode(foos, &nodes[5], NULL)));
		ASSERT_TRUE(MclHashMap_FindNode(foos, 5) == NULL);
		ASSERT_EQ(3, MclHashMap_GetCount(foos));

		MclHashNode node = MCL_NODE(6, (MclHashValue)6);
		ASSERT_TRUE(MCL_FAILED(MclHashMap_RemoveNode(foos, &node, NULL)));

        ASSERT_EQ(3, MclHashMap_GetCount(foos));
	}
};
