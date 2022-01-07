#ifndef H81102F3A_EA60_4478_8DDB_B6A20D88388C
#define H81102F3A_EA60_4478_8DDB_B6A20D88388C

#include "example/domain/entity/object_id.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclObject);

MclObjectId MclObject_GetId(const MclObject*);
uint64_t MclObject_GetValue(const MclObject*);

MclStatus MclObject_DoubleValue(MclObject*);

MCL_STDC_END

#endif
