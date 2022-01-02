#ifndef H03F50EA8_2E58_4F87_81A5_B263DE8C6D71
#define H03F50EA8_2E58_4F87_81A5_B263DE8C6D71

#include "mcl/array/link_array.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

/////////////////////////////////////////////////////////////////////////////
#define MCL_ALLOCATOR_TYPE_DEF(ALLOCATOR_TYPE, TYPE, CAPACITY)              \
typedef struct ALLOCATOR_TYPE {                                        		\
    MclLinkArray elems;                                                     \
    uint8_t buff[CAPACITY * MCL_ALIGN_SIZE(sizeof(TYPE))];              	\
} ALLOCATOR_TYPE;                                                      		\
                                                                            \
MCL_INLINE MclStatus ALLOCATOR_TYPE##_Init(ALLOCATOR_TYPE *self) {      	\
    MCL_ASSERT_VALID_PTR(self);                                             \
    MCL_ASSERT_SUCC_CALL(MclLinkArray_Init(&self->elems, CAPACITY, MCL_ALIGN_SIZE(sizeof(TYPE)), self->buff)); \
    return MCL_SUCCESS;                                                     \
}                                                                           \
                                                                            \
MCL_INLINE TYPE* ALLOCATOR_TYPE##_Alloc(ALLOCATOR_TYPE *self) {         	\
    MCL_ASSERT_VALID_PTR_NIL(self);                                         \
    return (TYPE*)MclLinkArray_Take(&self->elems);                          \
}                                                                           \
                                                                            \
MCL_INLINE void ALLOCATOR_TYPE##_Free(ALLOCATOR_TYPE *self, TYPE *p) {      \
    MCL_ASSERT_VALID_PTR_VOID(self);                                        \
    MCL_ASSERT_VALID_PTR_VOID(p);                                           \
    MclLinkArray_Give(&self->elems, p);                                     \
}
/////////////////////////////////////////////////////////////////////////////

MCL_STDC_END

#endif
