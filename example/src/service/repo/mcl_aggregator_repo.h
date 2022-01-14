#ifndef H9409F07C_59C1_42E9_982B_51106B42024C
#define H9409F07C_59C1_42E9_982B_51106B42024C

#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);

void MclAggregatorRepo_Init();
void MclAggregatorRepo_Destroy();

MclStatus   MclAggregatorRepo_Insert(MclAggregator*);
MclAggregator*  MclAggregatorRepo_Remove(MclAggregatorId);

MclAggregator* MclAggregatorRepo_Fetch(MclAggregatorId);
const MclAggregator* MclAggregatorRepo_FetchConst(MclAggregatorId);

typedef bool (*MclAggregatorPred)(const MclAggregator*, void*);
MclAggregator* MclAggregatorRepo_FetchBy(MclAggregatorPred, void*);
const MclAggregator* MclAggregatorRepo_FetchConstBy(MclAggregatorPred, void*);

bool    MclAggregatorRepo_HasAggregator(MclAggregatorId);
bool    MclAggregatorRepo_IsEmpty();
MclSize MclAggregatorRepo_GetSize();

typedef MclStatus (*MclAggregatorVisit)(MclAggregator*, void*);
MclStatus MclAggregatorRepo_Accept(MclAggregatorVisit, void*);

typedef MclStatus (*MclAggregatorVisitConst)(const MclAggregator*, void*);
MclStatus MclAggregatorRepo_AcceptConst(MclAggregatorVisitConst, void*);

MCL_STDC_END

#endif
