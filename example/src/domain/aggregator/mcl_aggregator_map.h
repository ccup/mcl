#ifndef H9409F07C_59C1_42E9_982B_51106B42024B
#define H9409F07C_59C1_42E9_982B_51106B42024B

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/map/hash_map.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);

typedef MclHashMap MclAggregatorMap;

void MclAggregatorMap_Init(MclAggregatorMap*);

typedef void (*MclAggregatorMapElemDestroy)(MclAggregator*);
void MclAggregatorMap_Destroy(MclAggregatorMap*, MclAggregatorMapElemDestroy);

MclStatus  MclAggregatorMap_Insert(MclAggregatorMap*, MclAggregator*);
MclAggregator* MclAggregatorMap_Remove(MclAggregatorMap*, MclAggregatorId);

bool   MclAggregatorMap_HasAggregator(const MclAggregatorMap*, MclAggregatorId);

MclAggregator* MclAggregatorMap_FindById(const MclAggregatorMap*, MclAggregatorId);

typedef bool (*MclAggregatorMapElemPred)(const MclAggregator*, void*);
MclAggregator* MclAggregatorMap_FindByPred(const MclAggregatorMap*, MclAggregatorMapElemPred, void*);

bool   MclAggregatorMap_IsEmpty(const MclAggregatorMap*);
size_t MclAggregatorMap_GetCount(const MclAggregatorMap*);

typedef MclStatus (*MclAggregatorMapElemVisit)(MclAggregator*, void*);
MclStatus MclAggregatorMap_Accept(const MclAggregatorMap*, MclAggregatorMapElemVisit, void*);

MCL_STDC_END

#endif
