#include <cctest/cctest.h>
#include <unordered_map>

FIXTURE(HashMapTest) {
    std::unordered_map<int, char> map;

    TEST("should add node to map") {
    }
};
