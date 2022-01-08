#ifndef H9409F07C_59C1_42E9_982B_51106B42024B
#define H9409F07C_59C1_42E9_982B_51106B42024B

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/list/list.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

typedef MclList MclEntityList;

void MclEntityList_Init(MclEntityList*);

typedef void (*MclEntityListDataDestroy)(MclEntity*);
void MclEntityList_Destroy(MclEntityList*, MclEntityListDataDestroy);

MclStatus  MclEntityList_Insert(MclEntityList*, MclEntity*);
MclEntity* MclEntityList_Remove(MclEntityList*, MclEntityId);

bool   MclEntityList_HasEntity(const MclEntityList*, MclEntityId);

MclEntity* MclEntityList_FindById(const MclEntityList*, MclEntityId);

typedef bool (*MclEntityListDataPred)(const MclEntity*, void*);
MclEntity* MclEntityList_FindByPred(const MclEntityList*, MclEntityListDataPred, void*);

bool   MclEntityList_IsEmpty(const MclEntityList*);
size_t MclEntityList_GetCount(const MclEntityList*);

typedef MclStatus (*MclEntityList_EntityVisit)(MclEntity*, void*);
MclStatus MclEntityList_Accept(const MclEntityList*, MclEntityList_EntityVisit, void*);

typedef MclStatus (*MclEntityList_EntityVisitConst)(const MclEntity*, void*);
MclStatus MclEntityList_AcceptConst(const MclEntityList*, MclEntityList_EntityVisitConst, void*);

MCL_STDC_END

#endif
