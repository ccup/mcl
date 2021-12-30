#include "mcl/mem/shared_ptr.h"
#include "mcl/lock/atom.h"
#include "mcl/mem/malloc.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

static const uint64_t MCL_SHARED_PTR_SENTINEL = 0xdeadc0de;

MCL_TYPE(MclSharedPtr) {
    uint64_t sentinel;
    MclSharedPtrDestructor dtor;
    void* dtorArg;
    MclAtom refCount;
    void* ptr;
};

MCL_PRIVATE size_t MclSharedPtr_HeaderSize() {
    return MclAlign_GetSizeOf(sizeof(MclSharedPtr));
}

MCL_PRIVATE bool MclSharedPtr_IsValid(MclSharedPtr* self, void *p) {
    return (self && self->sentinel == MCL_SHARED_PTR_SENTINEL && self->ptr == p);
}

MCL_PRIVATE MclSharedPtr* MclSharedPtr_GetSelf(void *p) {
    MclSharedPtr *self = (MclSharedPtr*)((uint8_t*)p - MclSharedPtr_HeaderSize());
    return MclSharedPtr_IsValid(self, p) ? self : NULL;
}

MCL_PRIVATE void MclSharedPtr_Init(MclSharedPtr *self, MclSharedPtrDestructor dtor, void* dtorArg) {
    self->sentinel = MCL_SHARED_PTR_SENTINEL;
    self->dtor = dtor;
    self->dtorArg = dtorArg;
    MclAtom_Set(&self->refCount, 1);
    self->ptr = (uint8_t*)self + MclSharedPtr_HeaderSize();
}

void* MclSharedPtr_Create(size_t size, MclSharedPtrDestructor dtor, void* dtorArg) {
    MCL_ASSERT_TRUE_NIL(size > 0);

    MclSharedPtr *self = MCL_MALLOC(MclSharedPtr_HeaderSize() + MclAlign_GetSizeOf(size));
    MCL_ASSERT_VALID_PTR_NIL(self);

    MclSharedPtr_Init(self, dtor, dtorArg);
    return self->ptr;
}

void  MclSharedPtr_Delete(void *ptr) {
    MCL_ASSERT_VALID_PTR_VOID(ptr);

    MclSharedPtr *self = MclSharedPtr_GetSelf(ptr);
    MCL_ASSERT_VALID_PTR_VOID(self);

    if (MclAtom_Sub(&self->refCount, 1)) return;

    if(self->dtor) self->dtor(ptr, self->dtorArg);
    MCL_FREE(self);
}

void* MclSharedPtr_Ref(void *ptr) {
    MCL_ASSERT_VALID_PTR_NIL(ptr);

    MclSharedPtr *self = MclSharedPtr_GetSelf(ptr);
    MCL_ASSERT_VALID_PTR_NIL(self);

    (void)MclAtom_Add(&self->refCount, 1);
    return ptr;
}
