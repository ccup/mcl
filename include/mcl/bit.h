#ifndef HE531412B_FC62_48BD_9975_81CFE4573F0F
#define HE531412B_FC62_48BD_9975_81CFE4573F0F

#include "mcl/stdtype.h"

#ifndef MCL_BIT_MASK
#define MCL_BIT_MASK(BIT_NUM)       				\
(((uint64_t)1 << BIT_NUM) - 1)
#endif

#ifndef MCL_BITS_GET_VALUE
#define MCL_BITS_GET_VALUE(target, offset, length)	\
(((target) >> (offset)) & MCL_BIT_MASK(length))
#endif

#ifndef MCL_BIT_IS_ON
#define MCL_BIT_IS_ON(target, offset) 				\
(MCL_BITS_GET_VALUE(target, offset, 1) > 0)
#endif

#endif
