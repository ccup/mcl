#include "aggregator/mcl_aggregator_map.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/assert.h"

void MclAggregatorMap_Init(MclAggregatorMap *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclHashMap_Init(self, MclHashNodeAllocator_GetDefault());
}

void MclAggregatorMap_Destroy(MclAggregatorMap *self, MclAggregatorMapElemDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashMap_Clear(self, (MclHashValueDestroy)destroy);
}

MclStatus  MclAggregatorMap_Insert(MclAggregatorMap *self, MclAggregator *aggregator) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(aggregator);

//	MCL_ASSERT_SUCC_CALL(MclHashMap_Set);
//
//	MclListNode *node = MclList_PushBack(self, entity);
//	MCL_ASSERT_VALID_PTR(node);
//	return MCL_SUCCESS;
}
//
//MCL_PRIVATE bool MclEntityIdPred_IsEqual(MclListData data, void *arg) {
//	return (*(MclEntityId*)arg) == MclEntity_GetId((MclEntity*)data);
//}
//
//MclEntity* MclEntityList_Remove(MclEntityList *self, MclEntityId id) {
//	MCL_ASSERT_VALID_PTR_NIL(self);
//	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));
//
//	return MclList_RemoveByPred(self, MclEntityIdPred_IsEqual, &id);
//}
//
//MclEntity* MclEntityList_FindById(const MclEntityList *self, MclEntityId id) {
//	MCL_ASSERT_VALID_PTR_NIL(self);
//	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));
//
//	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id);
//}
//
//typedef struct {
//	MclEntityListElemPred pred;
//	void *arg;
//} MclEntityPred;
//
//MCL_PRIVATE bool MclEntityPred_Pred(MclListData data, void *arg) {
//	MclEntityPred *pred = (MclEntityPred*)arg;
//	return pred->pred((MclEntity*)data, pred->arg);
//}
//
//MclEntity*  MclEntityList_FindByPred(const MclEntityList *self, MclEntityListElemPred entityPred, void *arg) {
//	MCL_ASSERT_VALID_PTR_NIL(self);
//	MCL_ASSERT_VALID_PTR_NIL(entityPred);
//
//	MclEntityPred pred = {.pred = entityPred, .arg = arg};
//	return MclList_FindByPred(self, MclEntityPred_Pred, &pred);
//}
//
//bool MclEntityList_HasEntity(const MclEntityList *self, MclEntityId id) {
//	MCL_ASSERT_VALID_PTR_BOOL(self);
//	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));
//
//	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id) != NULL;
//}
//
//bool MclEntityList_IsEmpty(const MclEntityList *self) {
//	MCL_ASSERT_VALID_PTR_R(self, true);
//	return MclList_IsEmpty(self);
//}
//
//size_t MclEntityList_GetCount(const MclEntityList *self) {
//	MCL_ASSERT_VALID_PTR_NIL(self);
//	return MclList_GetCount(self);
//}
//
//typedef struct {
//	MclEntityListElemVisit visit;
//	void* arg;
//} MclEntityVisitor;
//
//MCL_PRIVATE MclStatus MclEntityVisitor_Visit(MclListData data, void *arg) {
//	MclEntityVisitor *visitor = (MclEntityVisitor*)arg;
//	return visitor->visit((MclEntity*)data, visitor->arg);
//}
//
//MclStatus MclEntityList_Accept(const MclEntityList *self, MclEntityListElemVisit visit, void *arg) {
//	MCL_ASSERT_VALID_PTR(self);
//	MCL_ASSERT_VALID_PTR(visit);
//
//	MclEntityVisitor visitor = {.visit = visit, .arg = arg};
//	return MclList_Accept(self, MclEntityVisitor_Visit, &visitor);
//}
