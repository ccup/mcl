#ifndef MCL_2A0884783E524E87B389A5EF72B3E856
#define MCL_2A0884783E524E87B389A5EF72B3E856

#include "mcl/typedef.h"

MCL_STDC_BEGIN

typedef uint64_t MclHashKey;
typedef void* MclHashValue;

typedef void (*MclHashValueDeleter)(MclHashValue);

MCL_TYPE_DEF(MclHashNode) {
    MclHashKey key;
    MclHashValue value;
};

MclHashNode* MclHashNode_Create(MclHashKey, MclHashValue);
void MclHashNode_Delete(MclHashNode*, MclHashValueDeleter);

MCL_STDC_END

#endif
