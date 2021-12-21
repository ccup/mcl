#ifndef MCL_9D4ABA5E943341339E3B717F62B7F8D1
#define MCL_9D4ABA5E943341339E3B717F62B7F8D1

#include "mcl/map/hash_node.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_FWD(MclHashMap);

MclHashMap* MclHashMap_Create(uint32_t bucketCount);
void MclHashMap_Delete(MclHashMap*, MclHashValueDeleter);

uint32_t MclHashMap_GetCount(const MclHashMap*);
bool MclHashMap_IsEmpty(const MclHashMap*);

void MclHashMap_Clear(MclHashMap*, MclHashValueDeleter);

MclStatus MclHashMap_Get(const MclHashMap*, MclHashKey, MclHashValue*);
MclStatus MclHashMap_Set(MclHashMap*, MclHashKey, MclHashValue);

void MclHashMap_Remove(MclHashMap*, MclHashKey, MclHashValueDeleter);

typedef bool (*MclHashNodePred)(MclHashNode, void *arg);
void MclHashMap_RemoveBy(MclHashMap*, MclHashNodePred, void *arg, MclHashValueDeleter);

typedef MclStatus (*MclHashNodeVisitor)(MclHashNode, void *arg);
MclStatus MclHashMap_Accept(const MclHashMap*, MclHashNodeVisitor, void *arg);

MCL_STDC_END

#endif
