#ifndef H5738CC4B_5AC3_4774_8B48_C397793DD517
#define H5738CC4B_5AC3_4774_8B48_C397793DD517

#include <atomic>
#include <string>

using FooId = uint32_t;

constexpr FooId FOO_ID_INVALID = 0xFFFFFFFF;

struct Foo {
	static std::atomic<uint16_t> FOO_COUNT;

    Foo(FooId id = FOO_ID_INVALID, uint32_t value = 0)
	: id{id}, value{0} {
        FOO_COUNT++;
    }

    ~Foo() {
        id = FOO_ID_INVALID;
        FOO_COUNT--;
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
