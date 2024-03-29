#include "repo/mcl_entity_repo.h"
#include "factory/mcl_entity_factory.h"
#include "entity/mcl_entity_list.h"
#include "mcl/lock/rwlock.h"
#include "mcl/lock/lockobj.h"

MCL_TYPE(MclEntityRepo) {
	MclRwLock rwlock;
	MclEntityList entities;
};

MCL_PRIVATE MclEntityRepo entityRepo = {
	.rwlock = MCL_RWLOCK(),
	.entities = MCL_LIST_DEFAULT(entityRepo.entities)
};

void MclEntityRepo_Init() {
	MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Init(&entityRepo.rwlock, NULL));
	MclEntityList_Init(&entityRepo.entities);
}

void MclEntityRepo_Destroy() {
	MclEntityList_Destroy(&entityRepo.entities, MclEntityFactory_Delete);
	MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Destroy(&entityRepo.rwlock));
}

MclStatus MclEntityRepo_Insert(MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(entity);

	MCL_LOCK_WRITE_AUTO(entityRepo.rwlock);
	MCL_ASSERT_SUCC_CALL(MclEntityList_Insert(&entityRepo.entities, entity));
	return MCL_SUCCESS;
}

MclEntity* MclEntityRepo_Remove(MclEntityId id) {
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MCL_LOCK_WRITE_AUTO(entityRepo.rwlock);
	return MclEntityList_Remove(&entityRepo.entities, id);
}

MclEntity* MclEntityRepo_Fetch(MclEntityId id) {
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MCL_LOCK_READ_AUTO(entityRepo.rwlock);

	MclEntity *result = MclEntityList_FindById(&entityRepo.entities, id);
	if (result) MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_WrLock(result));
	return result;
}

const MclEntity* MclEntityRepo_FetchConst(MclEntityId id) {
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(id));

	MCL_LOCK_READ_AUTO(entityRepo.rwlock);

	MclEntity *result = MclEntityList_FindById(&entityRepo.entities, id);
	if (result) MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_RdLock(result));
	return result;
}

typedef struct {
	MclEntityPred pred;
	void *arg;
} MclEntityRepoPred;

MCL_PRIVATE bool MclEntityRepoPred_LockPred(const MclEntity *entity, void *arg) {
	MclEntityRepoPred *repoPred = (MclEntityRepoPred*)arg;
	MCL_ASSERT_SUCC_CALL_BOOL(MclLockObj_RdLock((void*)entity));
	bool result = repoPred->pred(entity, repoPred->arg);
	MCL_ASSERT_SUCC_CALL_BOOL(MclLockObj_UnLock((void*)entity));
	return result;
}

MclEntity* MclEntityRepo_FetchBy(MclEntityPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MCL_LOCK_READ_AUTO(entityRepo.rwlock);

	MclEntityRepoPred repoPred = {.pred = pred, .arg = arg};
	MclEntity *result = MclEntityList_FindByPred(&entityRepo.entities, MclEntityRepoPred_LockPred, &repoPred);
	if (result) MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_WrLock(result));
	return result;
}

const MclEntity* MclEntityRepo_FetchConstBy(MclEntityPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MCL_LOCK_READ_AUTO(entityRepo.rwlock);

	MclEntityRepoPred repoPred = {.pred = pred, .arg = arg};
	MclEntity *result = MclEntityList_FindByPred(&entityRepo.entities, MclEntityRepoPred_LockPred, &repoPred);
	if (result) MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_RdLock(result));
	return result;
}

bool MclEntityRepo_HasEntity(MclEntityId id) {
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(id));

	MCL_LOCK_READ_AUTO(entityRepo.rwlock);
	return MclEntityList_HasEntity(&entityRepo.entities, id);
}

bool MclEntityRepo_IsEmpty() {
	MCL_LOCK_READ_AUTO(entityRepo.rwlock);
	return MclEntityList_IsEmpty(&entityRepo.entities);
}

MclSize MclEntityRepo_GetSize() {
	MCL_LOCK_READ_AUTO(entityRepo.rwlock);
	return MclEntityList_GetSize(&entityRepo.entities);
}

typedef struct {
	MclEntityVisit visit;
	void *arg;
} MclEntityRepoVisitor;

MCL_PRIVATE MclStatus MclEntityRepoVisitor_LockVisit(MclEntity *entity, void *arg, MclStatus(*lock)(void*)) {
	MclEntityRepoVisitor *visitor = (MclEntityRepoVisitor*)arg;

	MclStatus result = MCL_FAILURE;

	MCL_ASSERT_SUCC_CALL(lock(entity));
	result = visitor->visit(entity, visitor->arg);
	MCL_ASSERT_SUCC_CALL(MclLockObj_UnLock(entity));
	return result;
}

MCL_PRIVATE MclStatus MclEntityRepoVisitor_Visit(MclEntity *entity, void *arg) {
	return MclEntityRepoVisitor_LockVisit(entity, arg, MclLockObj_WrLock);
}

MCL_PRIVATE MclStatus MclEntityRepoVisitor_VisitConst(const MclEntity *entity, void *arg) {
	return MclEntityRepoVisitor_LockVisit((MclEntity*)entity, arg, MclLockObj_RdLock);
}

MclStatus MclEntityRepo_Accept(MclEntityVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityRepoVisitor visitor = {.visit = visit, .arg = arg};
	MCL_LOCK_READ_AUTO(entityRepo.rwlock);
	return MclEntityList_Accept(&entityRepo.entities, MclEntityRepoVisitor_Visit, &visitor);
}

MclStatus MclEntityRepo_AcceptConst(MclEntityVisitConst visit, void *arg) {
	MCL_ASSERT_VALID_PTR(visit);

	MclEntityRepoVisitor visitor = {.visit = (MclEntityVisit)visit, .arg = arg};
	MCL_LOCK_READ_AUTO(entityRepo.rwlock);
	return MclEntityList_AcceptConst(&entityRepo.entities, MclEntityRepoVisitor_VisitConst, &visitor);
}
