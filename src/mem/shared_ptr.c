#include "mcl/mem/shared_ptr.h"
#include "mcl/task/atom.h"

static const uint64_t MCL_SHARED_PTR_SENTINEL = 0xdeadc0de;

MCL_TYPE(MclSharedPtr) {
    uint64_t sentinel;
    MclSharedPtrDestructor dtor;
    MclAtom refCount;
    uint8_t obj[];
};

void* MclSharedPtr_Create(size_t size, MclSharedPtrDestructor destructor) {
    return NULL;
}

void  MclSharedPtr_Delete(void *ptr) {

}

void* MclSharedPtr_Ref(void *ptr) {
    return NULL;
}
