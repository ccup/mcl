#include "mcl/map/hash_map.h"
#include "mcl/list/list.h"
#include "mcl/list/list_node_allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

typedef MclList MclHashBucket;

MCL_TYPE(MclHashMap) {
    uint32_t count;
    uint32_t bucketCount;
    MclHashBucket* buckets[];
};

//MCL_PRIVATE void MclHashBucket_Clear(MclHashBucket *self) {
//
//}

MCL_PRIVATE void MclHashMap_Init(MclHashMap *self, uint32_t bucketCount) {
    self->bucketCount = bucketCount;
    self->count = 0;
    for (uint32_t i = 0; i < bucketCount; i++) {
        self->buckets[i] = MclList_CreateDefault();
    }
}

MCL_PRIVATE uint32_t MclHashMap_GetBucketId(const MclHashMap *self, MclHashKey key)
{
    return key % self->bucketCount;
}

MclHashMap* MclHashMap_Create(uint32_t bucketCount) {
    MCL_ASSERT_TRUE_NIL(bucketCount > 0);

    MclHashMap *self = MCL_MALLOC(sizeof(MclHashMap) + sizeof(MclHashBucket*) * bucketCount);
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclHashMap_Init(self, bucketCount);
    return self;
}

void MclHashMap_Delete(MclHashMap *self, MclHashValueDeleter valueDeleter, void *delArg) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashMap_Clear(self, valueDeleter, delArg);
    MCL_FREE(self);
}

uint32_t MclHashMap_GetCount(const MclHashMap *self) {
    return self ? self->count : 0;
}

bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetCount(self) == 0;
}

void MclHashMap_Clear(MclHashMap *self, MclHashValueDeleter valueDeleter, void *delArg) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    for (uint32_t i = 0; i < self->bucketCount; i++) {
//        MclList_Clear(self->buckets[i], valueDeleter);
    }
    self->count = 0;
}

MclStatus MclHashMap_Get(const MclHashMap *self, MclHashKey key, MclHashValue *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);

    uint32_t bucketId = MclHashMap_GetBucketId(self, key);

    MclListNode *node = NULL;
    MCL_LIST_FOREACH(self->buckets[bucketId], node) {
        MclHashNode *hashNode = (MclHashNode*)(node->data);
        if (hashNode->key == key) {
            *value = hashNode->value;
            return MCL_SUCCESS;
        }
    }
    return MCL_FAILURE;
}

MclStatus MclHashMap_Set(MclHashMap *self, MclHashKey key, MclHashValue value) {
    MCL_ASSERT_VALID_PTR(self);

    MclHashNode *hashNode = MclHashNode_Create(key, value);
    MCL_ASSERT_VALID_PTR(hashNode);

    uint32_t bucketId = MclHashMap_GetBucketId(self, key);
    MCL_ASSERT_SUCC_CALL(MclList_PushBack(self->buckets[bucketId], hashNode));
    return MCL_SUCCESS;
}

MCL_PRIVATE bool MclHashMap_IsKeyEqual(MclListData data, void *arg) {
    MclHashKey *key = (MclHashKey*)arg;
    MclHashNode *node = (MclHashNode*)data;
    return node->key == *key;
}

//MCL_PRIVATE void MclHashMap_DeleteHashNode()

void MclHashMap_Remove(MclHashMap *self, MclHashKey key, MclHashValueDeleter valueDeleter, void *delArg) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    uint32_t bucketId = MclHashMap_GetBucketId(self, key);
//    MclList_RemoveBy(self->buckets[bucketId], MclHashMap_IsKeyEqual, NULL);
}

void MclHashMap_RemoveBy(MclHashMap *self, MclHashNodePred pred, void *predArg, MclHashValueDeleter valueDeleter, void *delArg) {

}

MclStatus MclHashMap_Accept(const MclHashMap *self, MclHashNodeVisitor visitor, void *arg) {
    return MCL_SUCCESS;
}
