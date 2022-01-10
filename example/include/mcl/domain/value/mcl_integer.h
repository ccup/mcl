#ifndef HF8EC0370_4D46_46EF_84A9_CFB40FD420D1
#define HF8EC0370_4D46_46EF_84A9_CFB40FD420D1

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MclInteger;

static const MclInteger MCL_INTEGER_MAX = MCL_UINT32_MAX;

MCL_INLINE void MclInteger_Clear(MclInteger *self) {
	if (self) (*self) = 0;
}

MCL_STDC_END

#endif
