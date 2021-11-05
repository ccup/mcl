#ifndef HF38173B9_6BA4_4858_AC8A_3B842A18F244
#define HF38173B9_6BA4_4858_AC8A_3B842A18F244

#include <stddef.h>

#define Mcl_ContainerOf(ptr, type, member) ({ \
		const typeof( ((type *)0)->member ) *__mptr = (ptr); \
		(type *)( (char *)__mptr - offsetof(type,member) );})

#endif
