#include "mcl/map/hash_node.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MclHashNode* MclHashNode_Create(MclHashKey key, MclHashValue value) {
    MclHashNode* self = MCL_MALLOC(sizeof(MclHashNode));
    MCL_ASSERT_VALID_PTR_NIL(self);

    self->key = key;
    self->value = value;
    return self;
}

void MclHashNode_Delete(MclHashNode *self, MclHashValueDeleter valueDeleter, void *delArg) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (valueDeleter && self->value) valueDeleter(self->value, delArg);
    MCL_FREE(self);
}
