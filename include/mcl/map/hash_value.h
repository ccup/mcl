#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21A
#define MCL_F2A54BFBC3EF41079397B09166E3F21A

#include "mcl/keyword.h"
#include "mcl/typedef.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef void* MclHashValue;

MCL_TYPE(MclHashValueDeleter) {
    void (*destroy)(MclHashValueDeleter*, MclHashValue);
};

MCL_INLINE void MclHashValueDeleter_Destroy(MclHashValueDeleter *deleter, MclHashValue value) {
    if (deleter && deleter->destroy) deleter->destroy(deleter, value);
}

///////////////////////////////////////////////////////////////
#define MCL_HASH_VALUE_DELETER(DESTROY) {.destroy = DESTROY}

MCL_STDC_END

#endif
