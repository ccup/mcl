#ifndef HFB29BCA2_4A0D_414A_BF4C_97FFFA140FC9
#define HFB29BCA2_4A0D_414A_BF4C_97FFFA140FC9

#include "mcl/keyword.h"
#include "mcl/assert.h"
#include "mcl/log/log.h"
#include <stdlib.h>
#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#endif

#define MCL_ARRAY_DECL(elem, name) 							\
typedef struct name{ elem* head; uint32_t capacity; uint32_t length;}name

#define MCL_ARRAY_INIT(arr, ptr, size, length)				\
MCL_STATIC_ASSERT(size >= length);							\
arr = {ptr, size, length}

#define MCL_ARRAY_INIT_LOCAL(arr, elem, size)	            \
elem __##arr[size] = {0};						            \
MCL_ARRAY_INIT(arr, __##arr, size, 0)

#define MCL_ARRAY_INIT_LOCAL_WITH_ELEMS(arr, elem, size, len, ...)\
elem __##arr[size] = __VA_ARGS__;						    \
MCL_ARRAY_INIT(arr, __##arr, size, len)

#define MCL_ARRAY_INIT_BY_ELEMS(arr, elem, ...)  			\
elem __##arr[] = __VA_ARGS__;								\
MCL_ARRAY_INIT(arr, __##arr, ARRAY_SIZE(__##arr), ARRAY_SIZE(__##arr))

#define MCL_ARRAY_RAW(arr)   	arr.head
#define MCL_ARRAY_SIZE(arr)  	arr.capacity
#define MCL_ARRAY_LEN(arr)   	arr.length
#define MCL_ARRAY_FIRST(arr) 	arr.head[0]
#define MCL_ARRAY_LAST(arr)  	arr.head[arr.length-1]
#define MCL_ARRAY_END(arr)  	arr.head[arr.length]
#define MCL_ARRAY_ELEM(arr, i) 	arr.head[i]
#define MCL_ARRAY_IS_EMPTY(arr) (arr.length <= 0)
#define MCL_ARRAY_IS_FULL(arr)  (arr.length >= arr.capacity)

#define MCL_ARRAY_GET_UNSAFE(arr, i, e)	e = arr.head[i]
#define MCL_ARRAY_PUT_UNSAFE(arr, i, e) arr.head[i] = e

#define MCL_ARRAY_GET(arr, i, e, nullobj)					    \
do {															\
	if (i < arr.length) {										\
		e = arr.head[i];										\
	} else {													\
		e = nullobj;											\
		MCL_LOG_WARN("get item in index %d out of array(%d:%d)", i, arr.capacity, arr.length);\
	}															\
} while(0)

#define MCL_ARRAY_PUT(arr, i, e)								\
do {															\
	if (i < arr.length) {										\
		arr.head[i] = e;										\
	} else {													\
		MCL_LOG_WARN("put item in index %d out of array(%d:%d)", i, arr.capacity, arr.length);\
	}															\
} while(0)

#define MCL_ARRAY_APPEND(arr, e)								\
do{																\
	if(arr.length < arr.capacity) {								\
		arr.head[arr.length++] = e;							    \
	} else {													\
		MCL_LOG_WARN("append to full array");						\
	}															\
} while(0)

#define MCL_ARRAY_MOVE_UNSAFE(arr, elem, start, len, offset)	\
memcpy(&arr.head[start + offset], &arr.head[start], sizeof(elem) * (len))

#define MCL_ARRAY_MOVE(arr, elem, start, len, offset)           \
do {															\
	if(len <= 0) break;										    \
	if(start >= arr.length) break;							    \
	if(start + len > arr.length) break;						    \
	if(offset + start < 0) break;								\
	if(offset + start + len > arr.capacity) break;				\
	MCL_ARRAY_MOVE_UNSAFE(arr, elem, start, len, offset);		\
} while(0)

#define MCL_ARRAY_INSERT(arr, elem, i, e)						\
do {															\
	if(MCL_ARRAY_IS_FULL(arr)) {								\
		MCL_LOG_WARN("insert to full array");						\
		break;													\
	}															\
	if(i == arr.length) {										\
		arr.head[arr.length++] = e;							    \
	} else if(i < arr.length) {									\
		MCL_ARRAY_MOVE(arr, elem, i, (arr.length - i) , 1);	\
		arr.head[i] = e;										\
		arr.length++;											\
	} else {													\
		MCL_LOG_WARN("insert item in index %d out of array(%d:%d)", i, arr.capacity, arr.length);\
	}															\
} while(0)

#define MCL_ARRAY_REMOVE(arr, elem, i)							\
do {															\
	if(i >= 0 && i < arr.length) {								\
		if (i < arr.length) {									\
			MCL_ARRAY_MOVE(arr, elem, i, (arr.length - i), -1);\
		}														\
        arr.length--;											\
	} else {													\
		MCL_LOG_WARN("remove item in index %d out of array(%d:%d)", i, arr.capacity, arr.length);\
    }															\
} while(0)

#define MCL_ARRAY_PUSH(arr, elem, e)				            \
do {															\
	if (arr.length < arr.capacity) {							\
		MCL_ARRAY_MOVE(arr, elem, 0, arr.length, 1);		    \
		arr.head[0] = e;							    		\
		arr.length++;											\
	} else {													\
		MCL_LOG_WARN("push to full array");						    \
	}															\
} while(0)

#define MCL_ARRAY_POP(arr, elem, e, nullobj)				    \
do {															\
	if (arr.length > 0) {										\
		e = MCL_ARRAY_FIRST(arr);								\
		MCL_ARRAY_MOVE(arr, elem, 0, arr.length, -1);			\
		arr.length--;											\
	} else {													\
		MCL_LOG_WARN("pop from empty array");						\
		e = nullobj;											\
	}															\
} while(0)

#define MCL_ARRAY_FIND_INDEX(index, arr, e)						\
do {															\
	int32_t __##index = arr.length;								\
	while(__##index >= 0) {										\
		if(arr.head[__##index]== e) {						    \
			break;												\
		}														\
		__##index--;											\
	}															\
	index = __##index;											\
} while(0)

#define MCL_ARRAY_FIND_INDEX_BY(index, arr, e, equal)		    \
do {															\
	int32_t __##index = arr.length;								\
	while(__##index >= 0) {										\
		if(equal(arr.head[__##index], e)) {						\
			break;												\
		}														\
		__##index--;											\
	}															\
	index = __##index;											\
} while(0)

#define MCL_ARRAY_INDEX_INVALID(index) (index < 0)

#define MCL_ARRAY_FOREACH(elem, arr, e)						    \
for(elem e = &MCL_ARRAY_FIRST(arr); e != &MCL_ARRAY_END(arr); ++e)

#endif
