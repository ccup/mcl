#ifndef H24A0810C_428D_4A2E_91A7_F949186D57BB
#define H24A0810C_428D_4A2E_91A7_F949186D57BB

#include "mcl/infra/allocator/mcl_allocator_type.h"

MCL_STDC_BEGIN

MCL_TYPE(MclFactoryAllocator) {
	void* (*alloc)(MclFactoryAllocator*, MclSize);
	void (*free)(MclFactoryAllocator*, void *obj);
	void (*destroyObj)(void *obj, void *arg);
	void *ctxt;
};

MCL_INLINE void* MclFactoryAllocator_Create(MclFactoryAllocator *self, MclSize size) {
	return self ? self->alloc(self, size) : NULL;
}

MCL_INLINE void MclFactoryAllocator_Delete(MclFactoryAllocator *self, void *ptr) {
	if (self && ptr) self->free(self, ptr);
}

void MclFactoryAllocator_Init(MclFactoryAllocator*, MclAllocatorType);

MCL_STDC_END

#endif
