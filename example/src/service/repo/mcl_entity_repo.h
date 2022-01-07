#ifndef H4211198B_18C6_4A41_BF4F_59D248E6B415
#define H4211198B_18C6_4A41_BF4F_59D248E6B415

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

void MclEntityRepo_Init();
void MclEntityRepo_Destroy();

MclStatus   MclEntityRepo_Insert(MclEntity*);
MclEntity*  MclEntityRepo_Remove(MclEntityId);

MclEntity* MclEntityRepo_Fetch(MclEntityId);
const MclEntity* MclEntityRepo_FetchConst(MclEntityId);

typedef MclEntityList_EntityPred MclEntity_Pred;
MclEntity* MclEntityList_FetchBy(MclEntity_Pred, void*);
const MclEntity* MclEntityList_FetchConstBy(MclEntity_Pred, void*);

bool   MclEntityRepo_HasEntity(MclEntityId);
bool   MclEntityRepo_IsEmpty();
size_t MclEntityRepo_GetSize();

typedef MclEntityList_EntityVisit MclEntity_Visit;
MclStatus MclEntityRepo_Accept(MclEntity_Visit, void*);

typedef MclEntityList_EntityVisitConst MclEntity_VisitConst;
MclStatus MclEntityRepo_AcceptConst(MclEntity_VisitConst, void*);

MCL_STDC_END

#endif
