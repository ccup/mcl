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

typedef bool (*MclEntityPred)(const MclEntity*, void*);
MclEntity* MclEntityRepo_FetchBy(MclEntityPred, void*);
const MclEntity* MclEntityRepo_FetchConstBy(MclEntityPred, void*);

bool    MclEntityRepo_HasEntity(MclEntityId);
bool    MclEntityRepo_IsEmpty();
MclSize MclEntityRepo_GetSize();

typedef MclStatus (*MclEntityVisit)(MclEntity*, void*);
MclStatus MclEntityRepo_Accept(MclEntityVisit, void*);

typedef MclStatus (*MclEntityVisitConst)(const MclEntity*, void*);
MclStatus MclEntityRepo_AcceptConst(MclEntityVisitConst, void*);

MCL_STDC_END

#endif
