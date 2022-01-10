#include "mcl/array/link_array.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

static const uint16_t MCL_LINK_ARRAY_INDEX_INVALID = MCL_UINT16_MAX;

MCL_PRIVATE void MclLinkArray_InitLink(MclLinkArray *self) {
    uint16_t count = MclArray_GetCapacity(&self->array);
    self->freeHead = 0;
    self->freeTail = count - 1;
    self->freeCount = count;
    uint16_t i = 0;
    MCL_ARRAY_FOREACH_INDEX(&self->array, i) {
        uint16_t *p = (uint16_t*)MclArray_Get(&self->array, i);
        (*p) = (i < (count - 1)) ? (i + 1) : MCL_LINK_ARRAY_INDEX_INVALID;
    }
}

MclStatus MclLinkArray_Init(MclLinkArray *self, uint16_t count, uint16_t elemBytes, uint8_t* buff) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(buff);
    MCL_ASSERT_TRUE(count > 0);
    MCL_ASSERT_TRUE(elemBytes >= sizeof(uint16_t));

    MCL_ASSERT_SUCC_CALL(MclArray_Init(&self->array, count, elemBytes, buff));
    MclLinkArray_InitLink(self);
    return MCL_SUCCESS;
}

void MclLinkArray_Clear(MclLinkArray *self) {
    MclArray_Clear(&self->array);
    MclLinkArray_InitLink(self);
}

MclLinkArray* MclLinkArray_Create(uint16_t count, uint16_t elemBytes) {
    MCL_ASSERT_TRUE_NIL(count > 0);
    MCL_ASSERT_TRUE_NIL(elemBytes >= sizeof(uint16_t));

    MclLinkArray *self = MCL_MALLOC(sizeof(MclLinkArray));
    MCL_ASSERT_VALID_PTR_NIL(self);

    uint8_t *buff = MCL_MALLOC(MclArray_GetBuffSize(count, elemBytes));
    if (!buff) {
        MCL_LOG_ERR("Malloc array for link buff failed!");
        MCL_FREE(self);
        return NULL;
    }

    if (MCL_FAILED(MclLinkArray_Init(self, count, elemBytes, buff))) {
        MCL_LOG_ERR("Init link array failed!");
        MCL_FREE(buff);
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclLinkArray_Delete(MclLinkArray *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (self->array.buff) {
        MCL_FREE(self->array.buff);
    }
    MCL_FREE(self);
}

void* MclLinkArray_Take(MclLinkArray *self) {
    MCL_ASSERT_VALID_PTR_NIL(self);
    if (self->freeHead == MCL_LINK_ARRAY_INDEX_INVALID) return NULL;

    uint16_t *p = (uint16_t*)MclArray_Get(&self->array, self->freeHead);
    MCL_ASSERT_VALID_PTR_NIL(p);

	self->freeHead = (*p);
    if(self->freeHead == MCL_LINK_ARRAY_INDEX_INVALID) {
    	self->freeTail = MCL_LINK_ARRAY_INDEX_INVALID;
    }
    self->freeCount--;
    return p;
}

MCL_PRIVATE uint16_t MclLinkArray_GetIndexOf(MclLinkArray *self, const void* p) {
    if(p == NULL) return MCL_LINK_ARRAY_INDEX_INVALID;

    uint8_t *begin = MclArray_Begin(&self->array);

    if (( (p < (void*)begin) || (p >= (void*)MclArray_End(&self->array))
        || (((uint8_t*)p - begin) % self->array.elemBytes) != 0)) {
		return MCL_LINK_ARRAY_INDEX_INVALID;
    }

    uint16_t index = (((uint8_t*)p - begin) / self->array.elemBytes);
    if (index >= MclArray_GetCapacity(&self->array)) return MCL_LINK_ARRAY_INDEX_INVALID;

    return index;
}

void MclLinkArray_Give(MclLinkArray *self, void *p) {

    uint16_t index = MclLinkArray_GetIndexOf(self, p);
    MCL_ASSERT_TRUE_VOID(index != MCL_LINK_ARRAY_INDEX_INVALID);

    if(self->freeHead == MCL_LINK_ARRAY_INDEX_INVALID) {
    	self->freeHead = index;
    }
    else {
        uint16_t *next = (uint16_t*)MclArray_Get(&self->array, self->freeTail);
        MCL_ASSERT_VALID_PTR_VOID(next);
        (*next) = index;
    }
    uint16_t *next = (uint16_t*)MclArray_Get(&self->array, index);
    MCL_ASSERT_VALID_PTR_VOID(next);
    (*next) = MCL_LINK_ARRAY_INDEX_INVALID;
    self->freeTail = index;
    self->freeCount++;
}
