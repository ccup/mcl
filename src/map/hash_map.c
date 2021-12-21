#include "mcl/map/hash_map.h"
#include "mcl/link/link.h"
#include "mcl/link/link_node_allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

typedef MclLink MclHashBucket;

MCL_TYPE_DEF(MclHashMap) {
    uint32_t count;
    uint32_t bucketCount;
    MclHashBucket* buckets[];
};

MCL_PRIVATE void MclHashMap_Init(MclHashMap *self, uint32_t bucketCount) {
    self->bucketCount = bucketCount;
    self->count = 0;
    for (uint32_t i = 0; i < bucketCount; i++) {
        self->buckets[i] = MclLink_CreateDefault();
    }
}

MclHashMap* MclHashMap_Create(uint32_t bucketCount) {
    MCL_ASSERT_TRUE_NIL(bucketCount > 0);

    MclHashMap *self = MCL_MALLOC(sizeof(MclHashMap) + sizeof(MclHashBucket*) * bucketCount);
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclHashMap_Init(self, bucketCount);
    return self;
}

void MclHashMap_Delete(MclHashMap *self, MclHashValueDeleter valueDeleter) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclHashMap_Clear(self, valueDeleter);
    MCL_FREE(self);
}

uint32_t MclHashMap_GetCount(const MclHashMap *self) {
    return self ? self->count : 0;
}

bool MclHashMap_IsEmpty(const MclHashMap *self) {
    return MclHashMap_GetCount(self) == 0;
}

void MclHashMap_Clear(MclHashMap *self, MclHashValueDeleter valueDeleter) {
    for (uint32_t i = 0; i < self->bucketCount; i++) {
        MclLink_Clear(self->buckets[i], valueDeleter);
    }
    self->count = 0;
}

MclStatus MclHashMap_Get(const MclHashMap *self, MclHashKey key, MclHashValue *value) {
    return MCL_SUCCESS;
}

MclStatus MclHashMap_Set(MclHashMap *self, MclHashKey key, MclHashValue value) {
    return MCL_SUCCESS;
}

void MclHashMap_Remove(MclHashMap *self, MclHashKey key, MclHashValueDeleter valueDeleter)
{

}

void MclHashMap_RemoveBy(MclHashMap *self, MclHashNodePred pred, void *arg, MclHashValueDeleter valueDeleter) {

}

MclStatus MclHashMap_Accept(const MclHashMap *self, MclHashNodeVisitor visitor, void *arg) {
    return MCL_SUCCESS;
}
