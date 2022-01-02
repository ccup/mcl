#include <cctest/cctest.h>
#include "mcl/map/hash_map.h"

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

	void Foo_HashDelete(MclHashValueDeleter *deleter, MclHashValue value) {
		auto f = (Foo*)value;
		if (f) Foo_Delete(f);
	}

    struct HashNodePred {
    	MclHashNodePred pred;
    	uint32_t id;
    };

    bool HashNodePred_IsEqual(MclHashNodePred *pred, const MclHashNode *node) {
    	HashNodePred *self = MCL_TYPE_REDUCT(pred, HashNodePred, pred);
    	auto foo = (Foo*)(node->value);
    	return self->id == foo->getId();
    }

    void HashNodePred_Init(HashNodePred *pred) {
    	pred->pred.pred = HashNodePred_IsEqual;
    	pred->id = FOO_ID_INVALID;
    }

    struct HashNodeVisitor {
    	MclHashNodeVisitor visitor;
    	uint32_t sum;
    };

    MclStatus HashNodeVisitor_Sum(MclHashNodeVisitor *visitor, MclHashNode *node) {
    	HashNodeVisitor *self = MCL_TYPE_REDUCT(visitor, HashNodeVisitor, visitor);
    	auto foo = (Foo*)(node->value);
    	if (FOO_ID_INVALID == foo->getId()) return MCL_STATUS_DONE;
    	self->sum += foo->getId();
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
    	fooDeleter.destroy = Foo_HashDelete;
    	HashNodePred_Init(&fooIdPred);
    	HashNodeVisitor_Init(&fooVisitor);
    }

    AFTER {
		MclHashMap_Delete(foos, &fooDeleter);
		ASSERT_EQ(0, Foo::FOO_COUNT.load());
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

		ASSERT_EQ(1, f->getId());
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
		ASSERT_EQ(1, f->getId());

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 2, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(2, f->getId());

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_Get(foos, 3, (MclHashValue*)(&f))));
		ASSERT_TRUE(f != nullptr);
		ASSERT_EQ(3, f->getId());
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
		MclHashMap_Set(foos, FOO_ID_INVALID, Foo_Create(FOO_ID_INVALID));
		MclHashMap_Set(foos, 15, Foo_Create(15));

//		MclHashMap_Dump(foos);

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
			ASSERT_EQ(i, f->getId());
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
