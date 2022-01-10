#ifndef H5738CC4B_5AC3_4774_8B48_C397793DD517
#define H5738CC4B_5AC3_4774_8B48_C397793DD517

#include "mcl/lock/atom.h"
#include <string>

using FooId = uint32_t;

constexpr FooId FOO_ID_INVALID = MCL_UINT32_MAX;

struct Foo {
	static MclAtom FOO_COUNT;

    Foo(FooId id = FOO_ID_INVALID, uint32_t value = 0)
	: id{id}, value{0} {
		MclAtom_Add(&FOO_COUNT, 1);
    }

    ~Foo() {
        id = FOO_ID_INVALID;
        MclAtom_Sub(&FOO_COUNT, 1);
    }

    FooId getId() const {
        return id;
    }

    uint32_t getValue() const {
    	return value;
    }

    std::string toString() const {
        return std::to_string(id);
    }

private:
    FooId id {FOO_ID_INVALID};
    uint32_t value;
};

#endif
