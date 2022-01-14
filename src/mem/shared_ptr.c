#include "mcl/mem/shared_ptr.h"
#include "mcl/lock/atom.h"
#include "mcl/mem/malloc.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

typedef struct {
    uint64_t sentinel;
    MclSharedPtrDestroy destroy;
    void* destroyArg;
    MclAtom refCount;
    void* ptr;
} MclSharedPtr;

MCL_PRIVATE const uint64_t MCL_SHARED_PTR_SENTINEL = 0xdeadc0de;

MCL_PRIVATE MclSize MclSharedPtr_HeaderSize() {
    return MclAlign_GetSizeOf(sizeof(MclSharedPtr));
}

MCL_PRIVATE bool MclSharedPtr_IsValid(MclSharedPtr* self, void *p) {
    return (self && self->sentinel == MCL_SHARED_PTR_SENTINEL && self->ptr == p);
}

MCL_PRIVATE MclSharedPtr* MclSharedPtr_GetSelf(void *p) {
    MclSharedPtr *self = (MclSharedPtr*)((uint8_t*)p - MclSharedPtr_HeaderSize());
    return MclSharedPtr_IsValid(self, p) ? self : NULL;
}

MCL_PRIVATE void MclSharedPtr_Init(MclSharedPtr *self, MclSharedPtrDestroy destroy, void* arg) {
    self->sentinel = MCL_SHARED_PTR_SENTINEL;
    self->destroy = destroy;
    self->destroyArg = arg;
    MclAtom_Set(&self->refCount, 1);
    self->ptr = (uint8_t*)self + MclSharedPtr_HeaderSize();
}

void* MclSharedPtr_Create(MclSize size, MclSharedPtrDestroy destroy, void* arg) {
    MCL_ASSERT_TRUE_NIL(size > 0);

    MclSharedPtr *self = MCL_MALLOC(MclSharedPtr_HeaderSize() + MclAlign_GetSizeOf(size));
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclSharedPtr_Init(self, destroy, arg);
    return self->ptr;
}

void  MclSharedPtr_Delete(void *ptr) {
    MCL_ASSERT_VALID_PTR_VOID(ptr);

    MclSharedPtr *self = MclSharedPtr_GetSelf(ptr);
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (MclAtom_SubFetch(&self->refCount, 1)) return;

    if(self->destroy) self->destroy(ptr, self->destroyArg);
    MCL_FREE(self);
}

void* MclSharedPtr_Ref(void *ptr) {
    MCL_ASSERT_VALID_PTR_NIL(ptr);

    MclSharedPtr *self = MclSharedPtr_GetSelf(ptr);
    MCL_ASSERT_VALID_PTR_NIL(self);

    (void)MclAtom_AddFetch(&self->refCount, 1);
    return ptr;
}
