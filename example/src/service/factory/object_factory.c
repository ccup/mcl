#include "../../../example/src/service/factory/object_factory.h"
#include "../../../example/src/domain/entity/private/object_private.h"
#include "mcl/lock/lockobj.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MclObject* MclObjectFactory_Create(MclObjectId id) {
	MclObject *self = MCL_MALLOC(sizeof(MclObject));
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclObject_Init(self, id))) {
		MCL_LOG_ERR("Initialize object (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}
	return self;
}

void MclObjectFactory_Delete(MclObject *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclObject_Destroy(self);
	MCL_FREE(self);
}

///////////////////////////////////////////////////////////
MclObject* MclObjectFactory_CreateSharedPtr(MclObjectId id) {

}

void MclObjectFactory_DeleteSharedPtr(MclObject *self) {

}

///////////////////////////////////////////////////////////
MCL_PRIVATE void MclObjectFactory_LockObjDestructor(void *obj, void *arg) {
	MclObject_Destroy((MclObject*)obj);
}

MclObject* MclObjectFactory_CreateLockObj(MclObjectId id) {
	MCL_LOCK_OBJ_CREATE(MclObject, self);
	MCL_ASSERT_VALID_PTR(self);

	if (MCL_FAILED(MclObject_Init(self, id))) {
		MCL_LOG_ERR("Initialize lock object (%u) failed!", id);
		MclLockObj_Delete(self, NULL, NULL);
		return NULL;
	}
	return self;
}

void MclObjectFactory_DeleteLockObj(MclObject *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclLockObj_Delete(self, MclObjectFactory_LockObjDestructor, NULL);
}

///////////////////////////////////////////////////////////
MclLockPtr MclObjectFactory_CreateLockPtr(MclObjectId id) {

}

void MclObjectFactory_DeleteLockObj(MclLockPtr ptr) {

}
