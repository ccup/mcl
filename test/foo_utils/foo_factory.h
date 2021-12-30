#ifndef H09F794ED_B8FC_4A49_A88A_5A8DA7B6FCE8
#define H09F794ED_B8FC_4A49_A88A_5A8DA7B6FCE8

#include "foo_utils/foo.h"
#include "mcl/lock/lockobj.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/list/list_data.h"
#include "mcl/map/hash_value.h"
#include "mcl/assert.h"

enum class FooCreateType {
	NORMAL, LOCKOBJ, SHARED_PTR
};

template<FooCreateType type = FooCreateType::NORMAL>
struct FooFactory {
    static Foo* create(FooId id = FOO_ID_INVALID) {
        return new Foo{id};
    }

    static void destroy(Foo *f) {
        delete f;
    }
};

template<>
struct FooFactory<FooCreateType::LOCKOBJ> {
    static Foo* create(FooId id) {
        auto p = (Foo*)MclLockObj_Create(sizeof(Foo));
        MCL_ASSERT_VALID_PTR_NIL(p);
        auto result = new(p) Foo(id);
        return result;
    }

    static void destroy(Foo *f) {
    	MclLockObj_Delete(f, FooFactory::fooDestroy, NULL);
    }

private:
    static void fooDestroy(void *p, void *arg) {
    	auto foo = (Foo*)p;
    	foo->~Foo();
    }
};

template<>
struct FooFactory<FooCreateType::SHARED_PTR> {
    static Foo* create(FooId id) {
        auto p = (Foo*)MclSharedPtr_Create(sizeof(Foo), fooDestroy, NULL);
        MCL_ASSERT_VALID_PTR_NIL(p);
        auto result = new(p) Foo(id);
        return result;
    }

    static void destroy(Foo *f) {
        MclSharedPtr_Delete(f);
    }

private:
    static void fooDestroy(void *p, void *arg) {
        auto foo = (Foo*)p;
        foo->~Foo();
    }
};

template<FooCreateType type>
void Foo_ListDelete(MclListDataDeleter *deleter, MclListData data) {
    auto f = (Foo*)data;
    if (f) FooFactory<type>::destroy(f);
}

template<FooCreateType type>
void Foo_HashDelete(MclHashValueDeleter *deleter, MclHashValue value) {
    auto f = (Foo*)value;
    if (f) FooFactory<type>::destroy(f);
}

MCL_INLINE void Foo_Destruct(void *f, void *arg) {
    auto foo = (Foo*)f;
    foo->~Foo();
}

#endif
