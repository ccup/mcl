#include "entity/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataVisitIntf visitIntf;
	MclEntityList_EntityVisit visit;
	void* arg;
} MclEntityVisitor;


MclStatus MclEntityVisitor_Visit(MclListDataVisitIntf *visitIntf, MclListData data) {
	MclEntityVisitor *self = MCL_TYPE_REDUCT(visitIntf, MclEntityVisitor, visitIntf);
	return self->visit((MclEntity*)data, self->arg);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataVisitIntf visitIntf;
	MclEntityList_EntityVisitConst visit;
	void* arg;
} MclEntityVisitorConst;


MclStatus MclEntityVisitorConst_Visit(MclListDataVisitIntf *visitIntf, MclListData data) {
	MclEntityVisitorConst *self = MCL_TYPE_REDUCT(visitIntf, MclEntityVisitorConst, visitIntf);
	return self->visit((const MclEntity*)data, self->arg);
}

///////////////////////////////////////////////////////////
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

MclStatus MclEntityList_Accept(const MclEntityList *self, MclEntityList_EntityVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityVisitor visitor = {.visitIntf = MCL_LIST_DATA_VISIT_INTF(MclEntityVisitor_Visit), .visit = visit, .arg = arg};
	return MclList_Accept(self, &visitor.visitIntf);
}

MclStatus MclEntityList_AcceptConst(const MclEntityList *self, MclEntityList_EntityVisitConst visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityVisitorConst visitor = {.visitIntf = MCL_LIST_DATA_VISIT_INTF(MclEntityVisitorConst_Visit), .visit = visit, .arg = arg};
	return MclList_Accept(self, &visitor.visitIntf);
}
