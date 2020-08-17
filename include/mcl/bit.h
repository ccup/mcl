#ifndef HE531412B_FC62_48BD_9975_81CFE4573F0F
#define HE531412B_FC62_48BD_9975_81CFE4573F0F

#include <stdint.h>

#ifndef BIT_MASK
#define BIT_MASK(BIT_NUM)       				\
(((uint64_t)1 << BIT_NUM) - 1)
#endif

#ifndef GET_BITS_VALUE
#define GET_BITS_VALUE(target, offset, length)	\
(((target) >> (offset)) & BIT_MASK(length))
#endif

#ifndef IS_BIT_ON
#define IS_BIT_ON(target, offset) 				\
(GET_BITS_VALUE(target, offset, 1) > 0)
#endif

#endif
