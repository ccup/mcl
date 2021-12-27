#ifndef MCL_7E11EC97E4EC40BA94ED0DDD26AB0436
#define MCL_7E11EC97E4EC40BA94ED0DDD26AB0436

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_INLINE size_t MclAlign_GetSizeOf(size_t size) {
    return (size + (sizeof(char *) - 1)) & ~(sizeof(char *) - 1);
}

MCL_STDC_END

#endif
