#include "entity/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataDestroyIntf destroyIntf;
	MclEntityList_EntityDestroy destroy;
} MclEntityDestroy;

MCL_PRIVATE void MclEntityDestroy_Destroy(MclListDataDestroyIntf *destroyIntf, MclListData data) {
	MclEntityDestroy *self = MCL_TYPE_REDUCT(destroyIntf, MclEntityDestroy, destroyIntf);
	self->destroy((MclEntity*)data);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataPredIntf predIntf;
	MclEntityId id;
} MclEntityIdPred;

bool MclEntityIdPred_IsEqual(MclListDataPredIntf *predIntf, MclListData data) {
	MclEntityIdPred *self = MCL_TYPE_REDUCT(predIntf, MclEntityIdPred, predIntf);
	return self->id == MclEntity_GetId((MclEntity*)data);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataPredIntf predIntf;
	MclEntityList_EntityPred pred;
	void* arg;
} MclEntityPred;

bool MclEntityPred_Pred(MclListDataPredIntf *predIntf, MclListData data) {
	MclEntityPred *self = MCL_TYPE_REDUCT(predIntf, MclEntityPred, predIntf);
	return self->pred((MclEntity*)data, self->arg);
}

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

void MclEntityList_Destroy(MclEntityList *self, MclEntityList_EntityDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclEntityDestroy deleter = {.destroyIntf = MCL_LIST_DATA_DESTROY_INTF(MclEntityDestroy_Destroy), .destroy = destroy};
	MclList_Clear(self, &deleter.destroyIntf);
}

MclStatus MclEntityList_Insert(MclEntityList *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);

	MCL_ASSERT_SUCC_CALL(MclList_PushBack(self, entity));
	return MCL_SUCCESS;
}

MclEntity* MclEntityList_Remove(MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED_INTF(MclEntityIdPred_IsEqual), .id = id};
	return MclList_RemoveByPred(self, &isIdEqual.predIntf);
}

MclEntity* MclEntityList_Find(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED_INTF(MclEntityIdPred_IsEqual), .id = id};
	return MclList_FindByPred(self, &isIdEqual.predIntf);
}

MclEntity*  MclEntityList_FindBy(const MclEntityList *self, MclEntityList_EntityPred entityPred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(entityPred);

	MclEntityPred pred = {.predIntf = MCL_LIST_DATA_PRED_INTF(MclEntityIdPred_IsEqual), .pred = entityPred, .arg = arg};
	return MclList_FindByPred(self, &pred.predIntf);
}

bool MclEntityList_HasEntity(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED_INTF(MclEntityIdPred_IsEqual), .id = id};
	return MclList_FindByPred(self, &isIdEqual.predIntf) != NULL;
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
