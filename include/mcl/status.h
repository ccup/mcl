#ifndef HA25033D6_1564_4748_B2C8_4DE2C5A286DE
#define HA25033D6_1564_4748_B2C8_4DE2C5A286DE

#include "mcl/stdc.h"
#include "mcl/keyword.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

typedef enum MclStatus {
	MCL_SUCCESS = 0,
	MCL_STATUS_DONE = 1,

	MCL_FAILURE = 0x80000001,
	MCL_NULLPTR,
	MCL_TIMEDOUT,
	MCL_OUT_OF_RANGE,
	MCL_UNINITIALIZED,
    MCL_UNIMPLEMENTED,
	MCL_FATAL_BUG,
} MclStatus;


MCL_INLINE bool MclStatus_IsOK(MclStatus status) {
    return MCL_SUCCESS == status;
}

MCL_INLINE bool MclStatus_IsFailed(MclStatus status) {
    return status >= MCL_FAILURE;
}

MCL_INLINE bool MclStatus_IsDone(MclStatus status) {
	return MCL_STATUS_DONE == status;
}

#define MCL_FAILED(result)   MclStatus_IsFailed(result)
#define MCL_DONE(result)     MclStatus_IsDone(result)

MCL_STDC_END

#endif
