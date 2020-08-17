#ifndef H03F50EA8_2E58_4F87_81A5_B263DE8C6D71
#define H03F50EA8_2E58_4F87_81A5_B263DE8C6D71

#include "mcl/keyword.h"
#include "mcl/assertion.h"
#include <stdint.h>
#include <stddef.h>

MCL_STDC_BEGIN

#define MCL_ALLOCATOR(TYPE)           TYPE##Allocator
#define MCL_ALLOCATOR_ALLOC(TYPE)     TYPE##AllocatorAlloc
#define MCL_ALLOCATOR_FREE(TYPE)      TYPE##AllocatorFree
#define MCL_ALLOCATOR_INIT(TYPE)      TYPE##AllocatorInit
#define MCL_ALLOCATOR_INST(TYPE)      TYPE##AllocatorInst
#define MCL_ALLOCATOR_TYPE(TYPE)      TYPE##AllocatorType
#define MCL_ALLOCATOR_ELEM(TYPE) 	  TYPE##AllocatorElemType
#define MCL_ALLOCATOR_HANDLE(TYPE) 	  TYPE##AllocatorHandle

#define MCL_DEF_ALLOCATOR(TYPE, MAX_NUM)						\
typedef union MCL_ALLOCATOR_ELEM(TYPE){							\
    char obj[sizeof(TYPE)];										\
    union MCL_ALLOCATOR_ELEM(TYPE)* next;					    \
}MCL_ALLOCATOR_ELEM(TYPE);						    			\
																\
typedef struct MCL_ALLOCATOR_TYPE(TYPE){						\
	MCL_ALLOCATOR_ELEM(TYPE) elems[MAX_NUM];	    			\
	MCL_ALLOCATOR_ELEM(TYPE)* freeHead;							\
	MCL_ALLOCATOR_ELEM(TYPE)* freeTail;							\
    uint16_t  freeCounter;										\
}MCL_ALLOCATOR_TYPE(TYPE);										\
																\
MCL_PRIVATE MCL_ALLOCATOR_TYPE(TYPE)* MCL_ALLOCATOR_INST(TYPE) = NULL;\
																\
MCL_PRIVATE void MCL_ALLOCATOR_INIT(TYPE)() {					\
	static MCL_ALLOCATOR_TYPE(TYPE) allocator;					\
	if (MCL_ALLOCATOR_INST(TYPE) != NULL) return;				\
	allocator.freeHead = NULL;									\
	allocator.freeTail = NULL;									\
	allocator.freeCounter = 0;									\
    for(uint16_t i = 0; i < MAX_NUM; i++) {						\
    	MCL_ALLOCATOR_ELEM(TYPE)* elem = &allocator.elems[i];	\
        if(allocator.freeHead == 0)	{							\
        	allocator.freeHead = elem;							\
        }														\
        else {													\
            MCL_ASSERT_VALID_PTR_VOID(allocator.freeTail);		\
            allocator.freeTail->next = elem;					\
        }														\
        allocator.freeTail = elem;								\
        allocator.freeTail->next = NULL;						\
        allocator.freeCounter++;								\
    }															\
    MCL_ALLOCATOR_INST(TYPE) = &allocator;						\
}																\
																\
MCL_PRIVATE TYPE* MCL_ALLOCATOR_ALLOC(TYPE)() {			        \
	MCL_ALLOCATOR_INIT(TYPE)();									\
	if (MCL_ALLOCATOR_INST(TYPE)->freeHead == NULL) {			\
		return NULL;											\
	}															\
	TYPE* p = (TYPE*)MCL_ALLOCATOR_INST(TYPE)->freeHead;		\
	MCL_ALLOCATOR_INST(TYPE)->freeHead = MCL_ALLOCATOR_INST(TYPE)->freeHead->next;\
    if(MCL_ALLOCATOR_INST(TYPE)->freeHead == NULL) {			\
    	MCL_ALLOCATOR_INST(TYPE)->freeTail = NULL;				\
    }															\
    MCL_ALLOCATOR_INST(TYPE)->freeCounter--;					\
    return p;													\
}																\
																\
MCL_PRIVATE void MCL_ALLOCATOR_FREE(TYPE)(void* p) {		    \
	if (MCL_ALLOCATOR_INST(TYPE) == NULL) return;				\
    if(p == NULL) return;										\
    MCL_ALLOCATOR_ELEM(TYPE)* elems = MCL_ALLOCATOR_INST(TYPE)->elems;\
    if (((((char*)p - (char*)elems) % sizeof(MCL_ALLOCATOR_ELEM(TYPE))) != 0)\
       ||(p < (void*)elems) || (p >= (void*)&(elems[MAX_NUM]))){\
    	MCL_ERR("free invalid pointer(%p)!", p);				\
		return;													\
    }															\
    MCL_ALLOCATOR_ELEM(TYPE)* elem = (MCL_ALLOCATOR_ELEM(TYPE)*)p;\
    if(MCL_ALLOCATOR_INST(TYPE)->freeHead == 0) {				\
    	MCL_ALLOCATOR_INST(TYPE)->freeHead = elem;				\
    }															\
    else {														\
        MCL_ASSERT_VALID_PTR_VOID(MCL_ALLOCATOR_INST(TYPE)->freeTail);\
        MCL_ALLOCATOR_INST(TYPE)->freeTail->next = elem;		\
    }															\
    MCL_ALLOCATOR_INST(TYPE)->freeTail = elem;					\
    MCL_ALLOCATOR_INST(TYPE)->freeTail->next = NULL;			\
    MCL_ALLOCATOR_INST(TYPE)->freeCounter++;					\
}																\
																\
typedef struct MCL_ALLOCATOR_HANDLE(TYPE){						\
	TYPE* (*alloc)();											\
	void  (*free)(void*);										\
}MCL_ALLOCATOR_HANDLE(TYPE);									\
																\
MCL_PRIVATE MCL_ALLOCATOR_HANDLE(TYPE) MCL_ALLOCATOR(TYPE) = {	\
	MCL_ALLOCATOR_ALLOC(TYPE), MCL_ALLOCATOR_FREE(TYPE)			\
}

MCL_STDC_END

#endif
