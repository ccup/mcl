#ifndef H6B23D917_47AC_4420_9E22_13EE1624D82C
#define H6B23D917_47AC_4420_9E22_13EE1624D82C

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/value/mcl_integer.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"

MCL_STDC_BEGIN

MCL_TYPE(MclEntity) {
	MclEntityId id;
	MclInteger value;
	MclAggregatorId aggregatorId;
};

bool MclEntityPrivate_IsOverflow(const MclEntity*, MclSize);

MCL_STDC_END

#endif
