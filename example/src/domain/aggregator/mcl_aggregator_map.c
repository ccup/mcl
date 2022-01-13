#include "aggregator/mcl_aggregator_map.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/assert.h"

void MclAggregatorMap_Init(MclAggregatorMap *self, MclHashBucket *bucket, uint32_t bucketCount) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclHashMap_Init(self, bucket, bucketCount, MclHashNodeAllocator_GetDefault());
}

void MclAggregatorMap_Destroy(MclAggregatorMap *self, MclAggregatorMapElemDestroy destroy) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclHashMap_Clear(self, (MclHashValueDestroy)destroy);
}

MclAggregator* MclAggregatorMap_Insert(MclAggregatorMap *self, MclAggregator *aggregator) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(aggregator);

	MclHashValue *value = MclHashMap_Set(self, MclAggregator_GetId(aggregator), aggregator);
	MCL_ASSERT_VALID_PTR_NIL(value);
	return (MclAggregator*)value;
}

MclAggregator* MclAggregatorMap_Remove(MclAggregatorMap *self, MclAggregatorId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(id));

	return MclHashMap_Remove(self, id);
}

bool MclAggregatorMap_HasAggregator(const MclAggregatorMap *self, MclAggregatorId id) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclAggregatorId_IsValid(id));

	return MclHashMap_Get(self, id) != NULL;
}

MclAggregator* MclAggregatorMap_FindById(const MclAggregatorMap *self, MclAggregatorId id) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (!MclAggregatorId_IsValid(id)) return NULL;

	return MclHashMap_Get(self, id);
}

MclAggregator*  MclAggregatorMap_FindByPred(const MclAggregatorMap *self, MclAggregatorMapElemPred pred, void *arg) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	MCL_ASSERT_VALID_PTR_NIL(pred);

	return MclHashMap_FindByPred(self, (MclHashNodePred)pred, arg);
}

bool MclAggregatorMap_IsEmpty(const MclAggregatorMap *self) {
	MCL_ASSERT_VALID_PTR_R(self, true);
	return MclHashMap_IsEmpty(self);
}

size_t MclAggregatorMap_GetSize(const MclAggregatorMap *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclHashMap_GetSize(self);
}

typedef struct {
	MclAggregatorMapElemVisit visit;
	void *arg;
} MclAggregatorMapVisitor;

MCL_PRIVATE MclStatus MclAggregatorMapVisitor_Visit(MclHashNode *hashNode, void *arg) {
	MclAggregatorMapVisitor *visitor = (MclAggregatorMapVisitor*)arg;
	MclAggregator *aggregator = (MclAggregator*)MclHashNode_GetValue(hashNode);
	return visitor->visit(aggregator, visitor->arg);
}

MclStatus MclAggregatorMap_Accept(const MclAggregatorMap *self, MclAggregatorMapElemVisit visit, void *arg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(visit);

	MclAggregatorMapVisitor visitor = {.visit = visit, .arg = arg};
	return MclHashMap_Accept(self, (MclHashNodeVisit)MclAggregatorMapVisitor_Visit, &visitor);
}

MclStatus MclAggregatorMap_AcceptConst(const MclAggregatorMap *self, MclAggregatorMapElemVisitConst visit, void *arg) {
	MCL_ASSERT_SUCC_CALL(MclAggregatorMap_Accept(self, (MclAggregatorMapElemVisit)visit, arg));
	return MCL_SUCCESS;
}
