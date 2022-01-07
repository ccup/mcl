#ifndef H9409F07C_59C1_42E9_982B_51106B42024B
#define H9409F07C_59C1_42E9_982B_51106B42024B

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/list/list.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

typedef MclList MclEntityList;

typedef void (*MclEntityList_EntityDestroy)(MclEntity*);

void MclEntityList_Init(MclEntityList*);
void MclEntityList_Destroy(MclEntityList*, MclEntityList_EntityDestroy);

MclStatus   MclEntityList_Insert(MclEntityList*, MclEntity*);
MclEntity*  MclEntityList_Remove(MclEntityList*, MclEntityId);
MclEntity*  MclEntityList_Find(MclEntityList*, MclEntityId);

bool   MclEntityList_HasEntity(const MclEntityList*, MclEntityId);
bool   MclEntityList_IsEmpty(const MclEntityList*);
size_t MclEntityList_GetSize(const MclEntityList*);

MCL_STDC_END

#endif
