#ifndef HE531412B_FC62_48BD_9975_81CFE4573F0F
#define HE531412B_FC62_48BD_9975_81CFE4573F0F

#include "mcl/stdtype.h"

#define MCL_BIT_UCHAR_NUM     8
#define MCL_BIT_USHORT_NUM    16
#define MCL_BIT_UINT32_NUM    32
#define MCL_BIT_UINT64_NUM    64

#define MCL_BIT_OFFSET(TYPE, OFFSET)        ((TYPE)1 << (OFFSET))

#define MCL_BIT_SET(DATA, OFFSET)           ((DATA) |= MCL_BIT_OFFSET(uint32_t, OFFSET))
#define MCL_BIT_CLR(DATA, OFFSET)           ((DATA) &= ~(MCL_BIT_OFFSET(uint32_t, OFFSET)))
#define MCL_BIT_IS_ON(DATA, OFFSET)         ((DATA) & MCL_BIT_OFFSET(uint32_t, OFFSET))

#define MCL_BIT_SET_64(DATA, OFFSET)        ((DATA) |= MCL_BIT_OFFSET(uint64_t, OFFSET))
#define MCL_BIT_CLR_64(DATA, OFFSET)        ((DATA) &= ~(MCL_BIT_OFFSET(uint64_t, OFFSET)))
#define MCL_BIT_IS_ON_64(DATA, OFFSET)      ((DATA) & MCL_BIT_OFFSET(uint64_t, OFFSET))

#define MCL_BITS_SET(DATA, MASK)            ((DATA) |= (MASK))
#define MCL_BITS_CLR(DATA, MASK)            ((DATA) &= ~(MASK))
#define MCL_BITS_NEG(DATA, MASK)            ((DATA) ^= (MASK))
#define MCL_BITS_ASSIGN(DATA, MASK, VALUE)  ((DATA) = ((DATA) & ~(MASK)) | ((VALUE) & (MASK)))

#define MCL_BITS_TEST(DATA, MASK)           ((DATA) & (MASK))
#define MCL_BITS_IS_ON(DATA, MASK)          (MCL_BITS_TEST(DATA, MASK) == (MASK))
#define MCL_BITS_IS_OFF(DATA, MASK)         (MCL_BITS_TEST(DATA, MASK) == 0)

#endif
