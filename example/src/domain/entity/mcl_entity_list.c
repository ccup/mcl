#include "entity/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

void MclEntityList_Init(MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclList_Init(self, MclListNodeAllocator_GetDefault());
}

void MclEntityList_Destroy(MclEntityList *self, MclEntityListDataDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclList_Clear(self, (MclListDataDestroy)destroy);
}

MclStatus MclEntityList_Insert(MclEntityList *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);

	MCL_ASSERT_SUCC_CALL(MclList_PushBack(self, entity));
	return MCL_SUCCESS;
}

MCL_PRIVATE bool MclEntityIdPred_IsEqual(MclListData data, void *arg) {
	return (*(MclEntityId*)arg) == MclEntity_GetId((MclEntity*)data);
}

MclEntity* MclEntityList_Remove(MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	return MclList_RemoveByPred(self, MclEntityIdPred_IsEqual, &id);
}

MclEntity* MclEntityList_FindById(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id);
}

typedef struct {
	MclEntityListDataPred pred;
	void *arg;
} MclEntityPred;

MCL_PRIVATE bool MclEntityPred_Pred(MclListData data, void *arg) {
	MclEntityPred *pred = (MclEntityPred*)arg;
	return pred->pred((MclEntity*)data, pred->arg);
}

MclEntity*  MclEntityList_FindByPred(const MclEntityList *self, MclEntityListDataPred entityPred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(entityPred);

	MclEntityPred pred = {.pred = entityPred, .arg = arg};
	return MclList_FindByPred(self, MclEntityPred_Pred, &pred);
}

bool MclEntityList_HasEntity(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));

	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id) != NULL;
}

bool MclEntityList_IsEmpty(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_R(self, true);
	return MclList_IsEmpty(self);
}

size_t MclEntityList_GetCount(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclList_GetCount(self);
}

typedef struct {
	MclEntityListDataVisit visit;
	void* arg;
} MclEntityVisitor;

MCL_PRIVATE MclStatus MclEntityVisitor_Visit(MclListData data, void *arg) {
	MclEntityVisitor *visitor = (MclEntityVisitor*)arg;
	return visitor->visit((MclEntity*)data, visitor->arg);
}

MclStatus MclEntityList_Accept(const MclEntityList *self, MclEntityListDataVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityVisitor visitor = {.visit = visit, .arg = arg};
	return MclList_Accept(self, MclEntityVisitor_Visit, &visitor);
}
