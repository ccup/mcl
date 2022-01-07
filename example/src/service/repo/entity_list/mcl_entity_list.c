#include "repo/entity_list/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataDeleter deleterIntf;
	MclEntityList_EntityDestroy destroy;
} MclEntityDeleter;

MCL_PRIVATE void MclEntityDeleter_Delete(MclListDataDeleter *deleter, MclListData data) {
	MclEntityDeleter *self = MCL_TYPE_REDUCT(deleter, MclEntityDeleter, deleterIntf);
	self->destroy((MclEntity*)data);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataPred predIntf;
	MclEntityId id;
} MclEntityIdPred;

bool MclEntityIdPred_IsEqual(MclListDataPred *pred, MclListData data) {
	MclEntityIdPred *self = MCL_TYPE_REDUCT(pred, MclEntityIdPred, predIntf);
	return self->id == MclEntity_GetId((MclEntity*)data);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataPred predIntf;
	MclEntityList_EntityPred pred;
	void* arg;
} MclEntityPred;

bool MclEntityPred_Pred(MclListDataPred *pred, MclListData data) {
	MclEntityPred *self = MCL_TYPE_REDUCT(pred, MclEntityPred, predIntf);
	return self->pred((MclEntity*)data, self->arg);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataVisitor visitorIntf;
	MclEntityList_EntityVisit visit;
	void* arg;
} MclEntityVisitor;


MclStatus MclEntityVisitor_Visit(MclListDataVisitor *visitor, MclListData data) {
	MclEntityVisitor *self = MCL_TYPE_REDUCT(visitor, MclEntityVisitor, visitorIntf);
	return self->visit((MclEntity*)data, self->arg);
}

///////////////////////////////////////////////////////////
typedef struct {
	MclListDataVisitor visitorIntf;
	MclEntityList_EntityVisitConst visit;
	void* arg;
} MclEntityVisitorConst;


MclStatus MclEntityVisitorConst_Visit(MclListDataVisitor *visitor, MclListData data) {
	MclEntityVisitorConst *self = MCL_TYPE_REDUCT(visitor, MclEntityVisitorConst, visitorIntf);
	return self->visit((const MclEntity*)data, self->arg);
}

///////////////////////////////////////////////////////////
void MclEntityList_Init(MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclList_Init(self, MclListNodeAllocator_GetDefault());
}

void MclEntityList_Destroy(MclEntityList *self, MclEntityList_EntityDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclEntityDeleter deleter = {.deleterIntf = MCL_LIST_DATA_DELETER(MclEntityDeleter_Delete), .destroy = destroy};
	MclList_Clear(self, &deleter.deleterIntf);
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

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED(MclEntityIdPred_IsEqual), .id = id};
	return MclList_RemoveFirst(self, &isIdEqual.predIntf, NULL);
}

MclEntity* MclEntityList_Find(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED(MclEntityIdPred_IsEqual), .id = id};
	return MclList_FindFirst(self, &isIdEqual.predIntf);
}

MclEntity*  MclEntityList_FindBy(const MclEntityList *self, MclEntityList_EntityPred entityPred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(entityPred);

	MclEntityPred pred = {.predIntf = MCL_LIST_DATA_PRED(MclEntityIdPred_IsEqual), .pred = entityPred, .arg = arg};
	return MclList_FindFirst(self, &pred.predIntf);
}

bool MclEntityList_HasEntity(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));

	MclEntityIdPred isIdEqual = {.predIntf = MCL_LIST_DATA_PRED(MclEntityIdPred_IsEqual), .id = id};
	return MclList_FindFirst(self, &isIdEqual.predIntf) != NULL;
}

bool MclEntityList_IsEmpty(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_R(self, true);
	return MclList_IsEmpty(self);
}

size_t MclEntityList_GetSize(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclList_GetCount(self);
}

MclStatus MclEntityList_Accept(const MclEntityList *self, MclEntityList_EntityVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityVisitor visitor = {.visitorIntf = MCL_LIST_DATA_VISITOR(MclEntityVisitor_Visit), .visit = visit, .arg = arg};
	return MclList_Accept(self, &visitor.visitorIntf);
}

MclStatus MclEntityList_AcceptConst(const MclEntityList *self, MclEntityList_EntityVisitConst visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityVisitorConst visitor = {.visitorIntf = MCL_LIST_DATA_VISITOR(MclEntityVisitorConst_Visit), .visit = visit, .arg = arg};
	return MclList_Accept(self, &visitor.visitorIntf);
}
