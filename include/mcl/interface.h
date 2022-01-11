#ifndef HE2B6CFCD_68D0_4823_A666_EEC611CC868E
#define HE2B6CFCD_68D0_4823_A666_EEC611CC868E

#include "mcl/typedef.h"

MCL_STDC_BEGIN

#define MCL_INTERFACE(TYPE)  MCL_TYPE(TYPE)

#define MCL_INTERFACE_CAST(PTR, TYPE, MEMBER) ({ 		\
	const typeof( ((TYPE*)0)->MEMBER ) *__mptr = (PTR); \
	(TYPE*)( (char *)__mptr - offsetof(TYPE, MEMBER));})

MCL_STDC_END

#endif
