#include "repo/entity_list/mcl_entity_list.h"
#include "mcl/assert.h"

typedef struct {
	MclListDataDeleter deleterIntf;
	MclEntityList_EntityDestroy destroy;
} MclEntityListDataDeleter;

MCL_PRIVATE void MclEntityList_DeleteEntity(MclListDataDeleter *intf, MclListData data) {
	MclEntityListDataDeleter *entityDeleter = MCL_TYPE_REDUCT(intf, MclEntityListDataDeleter, deleterIntf);
	entityDeleter->destroy((MclEntity*)data);
}

void MclEntityList_Init(MclEntityList *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclList_Init(self, MclListNodeAllocator_GetDefault());
}

void MclEntityList_Destroy(MclEntityList *self, MclEntityList_EntityDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclEntityListDataDeleter deleter = {.deleterInft = MCL_LIST_DATA_DELETER(MclEntityList_DeleteEntity), .destroy = destroy};
	MclList_Clear(self, &deleter);
}

MclStatus MclEntityList_Insert(MclEntityList *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);

	MCL_ASSERT_SUCC_CALL(MclList_PushBack(self, entity));
	return MCL_SUCCESS;
}

MclEntity*  MclEntityList_Remove(MclEntityList *self, MclEntityId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));


}

MclEntity*  MclEntityList_Find(MclEntityList*, MclEntityId) {

}

bool   MclEntityList_HasEntity(const MclEntityList*, MclEntityId) {

}

bool   MclEntityList_IsEmpty(const MclEntityList*) {

}

size_t MclEntityList_GetSize(const MclEntityList*) {

}
