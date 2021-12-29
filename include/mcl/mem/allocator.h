#ifndef H03F50EA8_2E58_4F87_81A5_B263DE8C6D71
#define H03F50EA8_2E58_4F87_81A5_B263DE8C6D71

#include "mcl/array/link_array.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

#define MCL_ALLOCATOR(TYPE)           TYPE##Allocator
#define MCL_ALLOCATOR_ELEM(TYPE) 	  TYPE##AllocatorElemType

#define MCL_ALLOCATOR_TYPE_DEF(TYPE, SIZE)                                  \
typedef union MCL_ALLOCATOR_ELEM(TYPE) {                                    \
    uint8_t obj[sizeof(TYPE)];                                              \
    void *p;                                                                \
} MCL_ALLOCATOR_ELEM(TYPE);                                                 \
                                                                            \
typedef struct MCL_ALLOCATOR(TYPE) {                                        \
    MclLinkArray elems;                                                     \
    uint8_t buff[SIZE * sizeof(MCL_ALLOCATOR_ELEM(TYPE))];                  \
} MCL_ALLOCATOR(TYPE);                                                      \
                                                                            \
MCL_INLINE void TYPE##Allocator_Init(MCL_ALLOCATOR(TYPE) *self) {           \
    MCL_ASSERT_VALID_PTR_VOID(self);                                        \
    MclLinkArray_Init(&self->elems, SIZE, sizeof(MCL_ALLOCATOR_ELEM(TYPE)), self->buff);\
}                                                                           \
                                                                            \
MCL_INLINE TYPE* TYPE##Allocator_Alloc(MCL_ALLOCATOR(TYPE) *self) {         \
    MCL_ASSERT_VALID_PTR_NIL(self);                                         \
    return (TYPE*)MclLinkArray_Take(&self->elems);                          \
}                                                                           \
                                                                            \
MCL_INLINE void TYPE##Allocator_Free(MCL_ALLOCATOR(TYPE) *self, TYPE *p) {  \
    MCL_ASSERT_VALID_PTR_VOID(self);                                        \
    MCL_ASSERT_VALID_PTR_VOID(p);                                           \
    MclLinkArray_Give(&self->elems, p);                                     \
}

MCL_STDC_END

#endif
