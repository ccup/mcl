#include "entity/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

void MclEntityList_Init(MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclList_Init(self, MclListNodeAllocator_GetDefault());
}

void MclEntityList_Destroy(MclEntityList *self, MclEntityListElemDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclList_Clear(self, (MclListDataDestroy)destroy);
}

MclStatus MclEntityList_Insert(MclEntityList *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);

	MclListNode *node = MclList_PushBack(self, entity);
	MCL_ASSERT_VALID_PTR(node);
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

bool MclEntityList_HasEntity(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));

	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id) != NULL;
}

MclEntity* MclEntityList_FindById(const MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	return MclList_FindByPred(self, MclEntityIdPred_IsEqual, &id);
}

MclEntity*  MclEntityList_FindByPred(const MclEntityList *self, MclEntityListElemPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	return MclList_FindByPred(self, (MclListDataPred)pred, arg);
}

bool MclEntityList_IsEmpty(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_R(self, true);
	return MclList_IsEmpty(self);
}

size_t MclEntityList_GetCount(const MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclList_GetCount(self);
}

MclStatus MclEntityList_Accept(const MclEntityList *self, MclEntityListElemVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	return MclList_Accept(self, (MclListDataVisit)visit, arg);
}

MclStatus MclEntityList_AcceptConst(const MclEntityList *self, MclEntityListElemVisitConst visit, void *arg) {
	MCL_ASSERT_SUCC_CALL(MclEntityList_Accept(self, (MclEntityListElemVisit)visit, arg));
	return MCL_SUCCESS;
}

