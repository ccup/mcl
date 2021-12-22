#ifndef HDD1B8843_1542_4D8A_ACBC_4D9719CA9B95
#define HDD1B8843_1542_4D8A_ACBC_4D9719CA9B95

#include "mcl/stdtype.h"

#define MCL_TYPE_DECL(TYPE)     \
typedef struct TYPE TYPE

#define MCL_TYPE(TYPE)	        \
MCL_TYPE_DECL(TYPE);			\
struct TYPE

#define MCL_TYPE_REDUCT(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#endif
