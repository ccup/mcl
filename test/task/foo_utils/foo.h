#ifndef H5738CC4B_5AC3_4774_8B48_C397793DD517
#define H5738CC4B_5AC3_4774_8B48_C397793DD517

#include <atomic>
#include <string>

using FooId = int;

constexpr FooId FOO_ID_INVALID = 0xFFFFFFFF;

struct Foo {
	static std::atomic<uint16_t> FOO_COUNT;

    Foo(FooId id) : id{id} {
        FOO_COUNT++;
    }

    ~Foo() {
        id = FOO_ID_INVALID;
        FOO_COUNT--;
    }

    FooId getId() const {
        return id;
    }

    std::string toString() const {
        return std::to_string(id);
    }

private:
    FooId id {FOO_ID_INVALID};
};

#endif
