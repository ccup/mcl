#ifndef H81102F3A_EA60_4478_8DDB_B6A20D88388C
#define H81102F3A_EA60_4478_8DDB_B6A20D88388C

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/value/mcl_integer.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

MclEntityId MclEntity_GetId(const MclEntity*);
MclInteger  MclEntity_GetValue(const MclEntity*);

MclStatus   MclEntity_DoubleValue(MclEntity*);

MCL_STDC_END

#endif
