#ifndef H9D2E73D9_46D0_4C0D_A563_2D6D5D085980
#define H9D2E73D9_46D0_4C0D_A563_2D6D5D085980

#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclAggregator);
MCL_TYPE_DECL(MclEntity);

MCL_TYPE(MclAggregateEntity) {
	MclAggregator *aggregator;
	MclEntity *entity;
};

MCL_TYPE(MclAggregateEntityConst) {
	const MclAggregator *aggregator;
	const MclEntity *entity;
};

void MclAggregateEntity_WrUnlock(MclAggregateEntity*);
void MclAggregateEntity_RdUnlock(MclAggregateEntityConst*);

///////////////////////////////////////////////////////////
#define MCL_LOCK_AGGREGATE_ENTITY_AUTO         MCL_RAII(MclAggregateEntity_WrUnlock)
#define MCL_LOCK_AGGREGATE_ENTITY_CONST_AUTO   MCL_RAII(MclAggregateEntity_RdUnlock)

MCL_STDC_END

#endif
