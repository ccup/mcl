#ifndef MCL_7E11EC97E4EC40BA94ED0DDD26AB0436
#define MCL_7E11EC97E4EC40BA94ED0DDD26AB0436

#include "mcl/keyword.h"

MCL_STDC_BEGIN

#define MCL_ALIGNED(N) __attribute__((aligned(N)))
#define MCL_ALIGN_SIZE(size) ((size + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1))

MCL_INLINE MclSize MclAlign_GetSizeOf(MclSize size) {
    return MCL_ALIGN_SIZE(size);
}

MCL_STDC_END

#endif
