#include "repo/mcl_aggregator_repo.h"
#include "aggregator/mcl_aggregator_map.h"
#include "aggregator/mcl_aggregator.h"
#include "factory/mcl_aggregator_factory.h"
#include "mcl/lock/rwlock.h"
#include "mcl/lock/lockobj.h"
#include "mcl/macro/repeat.h"

MCL_TYPE(MclAggregatorRepo) {
	MclRwLock rwlock;
	MclAggregatorMap aggregators;
	MclHashBucket buckets[MCL_HASHMAP_BUCKET_COUNT_DEFAULT];
};

#define __MCL_HASH_BUCKETS_ITEM_INIT(n)  [n]=MCL_HASH_BUCKET(aggregatorRepo.buckets[n]),

MCL_PRIVATE MclAggregatorRepo aggregatorRepo = {
	.buckets = { MCL_MACRO_REPEAT_SIMPLE(MCL_HASHMAP_BUCKET_COUNT_DEFAULT, __MCL_HASH_BUCKETS_ITEM_INIT)},
	.aggregators = MCL_HASHMAP_DEFAULT(aggregatorRepo.aggregators, aggregatorRepo.buckets, MCL_HASHMAP_BUCKET_COUNT_DEFAULT),
	.rwlock = MCL_RWLOCK(),
};

void MclAggregatorRepo_Init() {
	MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Init(&aggregatorRepo.rwlock, NULL));
	MclAggregatorMap_Init(&aggregatorRepo.aggregators, aggregatorRepo.buckets, MCL_HASHMAP_BUCKET_COUNT_DEFAULT);
}

void MclAggregatorRepo_Destroy() {
	MclAggregatorMap_Destroy(&aggregatorRepo.aggregators, MclAggregatorFactory_DeleteLockObj);
	MCL_ASSERT_SUCC_CALL_VOID(MclRwLock_Destroy(&aggregatorRepo.rwlock));
}

MclStatus MclAggregatorRepo_Insert(MclAggregator *aggregator) {
	MCL_ASSERT_VALID_PTR(aggregator);

	MclAggregator *result = NULL;

	MCL_LOCK_WRITE_SCOPE(aggregatorRepo.rwlock) {
		result = MclAggregatorMap_Insert(&aggregatorRepo.aggregators, aggregator);
	}

	MCL_ASSERT_VALID_PTR(result);
	if(result != aggregator) {
		MclAggregatorFactory_DeleteLockObj(result);
		MCL_LOG_WARN("Replace aggregator of key (%u)!", MclAggregator_GetId(aggregator));
	}
	return MCL_SUCCESS;
}

MclAggregator* MclAggregatorRepo_Remove(MclAggregatorId id) {
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(id));

	MCL_LOCK_WRITE_AUTO(aggregatorRepo.rwlock);
	return MclAggregatorMap_Remove(&aggregatorRepo.aggregators, id);
}

MclAggregator* MclAggregatorRepo_Fetch(MclAggregatorId id) {
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(id));

	MclAggregator *result = NULL;
	MCL_LOCK_READ_SCOPE(aggregatorRepo.rwlock) {
		result = MclAggregatorMap_FindById(&aggregatorRepo.aggregators, id);
		MCL_ASSERT_VALID_PTR_NIL(result);
		MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_WrLock(result));
	}
	return result;
}

const MclAggregator* MclAggregatorRepo_FetchConst(MclAggregatorId id) {
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(id));

	const MclAggregator *result = NULL;
	MCL_LOCK_READ_SCOPE(aggregatorRepo.rwlock) {
		result = MclAggregatorMap_FindById(&aggregatorRepo.aggregators, id);
		MCL_ASSERT_VALID_PTR_NIL(result);
		MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_RdLock((void*)result));
	}
	return result;
}

MclAggregator* MclAggregatorRepo_FetchBy(MclAggregatorPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(pred);

	MclAggregator *result = NULL;
	MCL_LOCK_READ_SCOPE(aggregatorRepo.rwlock) {
		result = MclAggregatorMap_FindByPred(&aggregatorRepo.aggregators, pred, arg);
		MCL_ASSERT_VALID_PTR_NIL(result);
		MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_WrLock(result));
	}
	return result;
}

const MclAggregator* MclAggregatorRepo_FetchConstBy(MclAggregatorPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(pred);

	const MclAggregator *result = NULL;
	MCL_LOCK_READ_SCOPE(aggregatorRepo.rwlock) {
		result = MclAggregatorMap_FindByPred(&aggregatorRepo.aggregators, pred, arg);
		MCL_ASSERT_VALID_PTR_NIL(result);
		MCL_ASSERT_SUCC_CALL_NIL(MclLockObj_RdLock((void*)result));
	}
	return result;
}

bool MclAggregatorRepo_HasAggregator(MclAggregatorId id) {
	MCL_ASSERT_TRUE_BOOL(MclAggregatorId_IsValid(id));

	MCL_LOCK_READ_AUTO(aggregatorRepo.rwlock);
	return MclAggregatorMap_HasAggregator(&aggregatorRepo.aggregators, id);
}

bool MclAggregatorRepo_IsEmpty() {
	MCL_LOCK_READ_AUTO(aggregatorRepo.rwlock);
	return MclAggregatorMap_IsEmpty(&aggregatorRepo.aggregators);
}

size_t MclAggregatorRepo_GeSize() {
	MCL_LOCK_READ_AUTO(aggregatorRepo.rwlock);
	return MclAggregatorMap_GetSize(&aggregatorRepo.aggregators);
}

typedef struct {
	MclAggregatorVisit visit;
	void *arg;
} MclAggregatorRepoVisitor;

MCL_PRIVATE MclStatus MclAggregatorRepoVisitor_Visit(MclAggregator *aggregator, void *arg) {
	MclAggregatorRepoVisitor *visitor = (MclAggregatorRepoVisitor*)arg;

	MclStatus result = MCL_FAILURE;

	MCL_ASSERT_SUCC_CALL(MclLockObj_WrLock(aggregator));
	result = visitor->visit(aggregator, visitor->arg);
	MCL_ASSERT_SUCC_CALL(MclLockObj_UnLock(aggregator));
	return result;
}

MclStatus MclAggregatorRepo_Accept(MclAggregatorVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(visit);

	MclAggregatorRepoVisitor visitor = {.visit = visit, .arg = arg};

	MCL_LOCK_READ_AUTO(aggregatorRepo.rwlock);

	return MclAggregatorMap_Accept(&aggregatorRepo.aggregators, MclAggregatorRepoVisitor_Visit, &visitor);
}

MCL_PRIVATE MclStatus MclAggregatorRepoVisitor_VisitConst(const MclAggregator *aggregator, void *arg) {
	MclAggregatorRepoVisitor *visitor = (MclAggregatorRepoVisitor*)arg;

	MclStatus result = MCL_FAILURE;

	MCL_ASSERT_SUCC_CALL(MclLockObj_RdLock((void*)aggregator));
	result = visitor->visit((MclAggregator*)aggregator, visitor->arg);
	MCL_ASSERT_SUCC_CALL(MclLockObj_UnLock((void*)aggregator));
	return result;
}

MclStatus MclAggregatorRepo_AcceptConst(MclAggregatorVisitConst visit, void *arg) {
	MCL_ASSERT_VALID_PTR(visit);

	MclAggregatorRepoVisitor visitor = {.visit = (MclAggregatorVisit)visit, .arg = arg};

	MCL_LOCK_READ_AUTO(aggregatorRepo.rwlock);

	return MclAggregatorMap_AcceptConst(&aggregatorRepo.aggregators, MclAggregatorRepoVisitor_VisitConst, &visitor);
}
