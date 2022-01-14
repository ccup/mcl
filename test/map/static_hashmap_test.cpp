#include <cctest/cctest.h>
#include "mcl/map/hash_map.h"
#include "mcl/macro/repeat.h"

namespace {
	#define __MCL_HASH_BUCKETS_ITEM_INIT(n)  [n]=MCL_HASH_BUCKET(buckets[n]),

    MclHashBucket buckets[MCL_HASHMAP_BUCKET_COUNT_DEFAULT] = {
    		MCL_MACRO_REPEAT_SIMPLE(MCL_HASHMAP_BUCKET_COUNT_DEFAULT, __MCL_HASH_BUCKETS_ITEM_INIT)
    };
    MclHashMap foomap = MCL_HASHMAP(foomap, buckets, MCL_HASHMAP_BUCKET_COUNT_DEFAULT, NULL);
}

FIXTURE(HashMapStaticTest) {
	constexpr static MclSize NODE_NUM = 6;

	MclHashNode nodes[NODE_NUM];
	MclHashMap *foos{nullptr};

    HashMapStaticTest() {
    	foos = &foomap;
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
		ASSERT_EQ(4, MclHashMap_GetSize(foos));

		long value{0};

		value = (long)MclHashMap_Get(foos, 1);
		ASSERT_EQ(1, value);

		value = (long)MclHashMap_Get(foos, 2);
		ASSERT_EQ(2, value);

		value = (long)MclHashMap_Get(foos, 4);
		ASSERT_EQ(0, value);

		auto result = MclHashMap_FindNode(foos, 5);
		ASSERT_TRUE(result != NULL);
		ASSERT_EQ(5, result->key);
		ASSERT_EQ(5, (long)result->value);

		ASSERT_TRUE(MclHashMap_FindNode(foos, 6) == NULL);

		ASSERT_TRUE(!MCL_FAILED(MclHashMap_RemoveNode(foos, &nodes[5], NULL)));
		ASSERT_TRUE(MclHashMap_FindNode(foos, 5) == NULL);
		ASSERT_EQ(3, MclHashMap_GetSize(foos));

		MclHashNode node = MCL_HASH_NODE(6, (MclHashValue)6);
		ASSERT_TRUE(MCL_FAILED(MclHashMap_RemoveNode(foos, &node, NULL)));

        ASSERT_EQ(3, MclHashMap_GetSize(foos));
	}
};
