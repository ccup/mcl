#ifndef H03F50EA8_2E58_4F87_81A5_B263DE8C6D71
#define H03F50EA8_2E58_4F87_81A5_B263DE8C6D71

#include "mcl/array/link_array.h"
#include "mcl/mem/align.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

#define __MCL_ALLOCATOR_INIT(ALLOCATOR_TYPE) ALLOCATOR_TYPE##_Init
#define __MCL_ALLOCATOR_ALLOC(ALLOCATOR_TYPE) ALLOCATOR_TYPE##_Alloc
#define __MCL_ALLOCATOR_FREE(ALLOCATOR_TYPE) ALLOCATOR_TYPE##_Free

/////////////////////////////////////////////////////////////////////////////////////
#define MCL_ALLOCATOR_TYPE_DEF(ALLOCATOR_TYPE, TYPE, TYPE_SIZE, CAPACITY)   		\
typedef struct ALLOCATOR_TYPE {                                        				\
    MclLinkArray elems;                                                     		\
    uint8_t buff[CAPACITY * MCL_ALIGN_SIZE(TYPE_SIZE)];              				\
} ALLOCATOR_TYPE;                                                      				\
                                                                            		\
MCL_INLINE MclStatus __MCL_ALLOCATOR_INIT(ALLOCATOR_TYPE)(ALLOCATOR_TYPE *self) { 	\
    MCL_ASSERT_VALID_PTR(self);                                             		\
    MCL_ASSERT_SUCC_CALL(MclLinkArray_Init(&self->elems, CAPACITY, MCL_ALIGN_SIZE(TYPE_SIZE), self->buff)); \
    return MCL_SUCCESS;                                                     		\
}                                                                           		\
                                                                            		\
MCL_INLINE TYPE* __MCL_ALLOCATOR_ALLOC(ALLOCATOR_TYPE)(ALLOCATOR_TYPE *self) {    	\
    MCL_ASSERT_VALID_PTR_NIL(self);                                         		\
    return (TYPE*)MclLinkArray_Take(&self->elems);                          		\
}                                                                           		\
                                                                            		\
MCL_INLINE void __MCL_ALLOCATOR_FREE(ALLOCATOR_TYPE)(ALLOCATOR_TYPE *self, TYPE *p){\
    MCL_ASSERT_VALID_PTR_VOID(self);                                        		\
    MCL_ASSERT_VALID_PTR_VOID(p);                                           		\
    MclLinkArray_Give(&self->elems, p);                                     		\
}
/////////////////////////////////////////////////////////////////////////////////////

#define MCL_ALLOCATOR_INIT(ALLOCATOR_TYPE, ALLOCATOR) 		\
	__MCL_ALLOCATOR_INIT(ALLOCATOR_TYPE)(&(ALLOCATOR))

#define MCL_ALLOCATOR_ALLOC(ALLOCATOR_TYPE, ALLOCATOR) 		\
	__MCL_ALLOCATOR_ALLOC(ALLOCATOR_TYPE)(&(ALLOCATOR))

#define MCL_ALLOCATOR_FREE(ALLOCATOR_TYPE, ALLOCATOR, PTR)	\
	__MCL_ALLOCATOR_FREE(ALLOCATOR_TYPE)(&(ALLOCATOR), (PTR))


MCL_STDC_END

#endif
