#ifndef HA25033D6_1564_4748_B2C8_4DE2C5A286DE
#define HA25033D6_1564_4748_B2C8_4DE2C5A286DE

#include "mcl/stdc.h"
#include "mcl/keyword.h"
#include "mcl/stdtype.h"

MCL_STDC_BEGIN

typedef enum {
	MCL_SUCCESS = 0,
	MCL_STATUS_DONE,
	MCL_STATUS_NOTHING_CHANGED,

	MCL_SUCCESS_CUNSTOMER_BEGIN = 0x10000000,

	MCL_FAILURE = 0x20000000,
	MCL_NULLPTR,
	MCL_TIMEDOUT,
	MCL_OUT_OF_RANGE,
	MCL_UNINITIALIZED,
    MCL_UNIMPLEMENTED,
	MCL_FATAL_BUG,

	MCL_FAILURE_CUSTOMER_BEGIN = 0x30000000,
} MclStatus;


MCL_INLINE bool MclStatus_IsSucc(MclStatus status) {
    return status == MCL_SUCCESS;
}

MCL_INLINE bool MclStatus_IsFailed(MclStatus status) {
    return status >= MCL_FAILURE;
}

MCL_INLINE bool MclStatus_IsDone(MclStatus status) {
	return status == MCL_STATUS_DONE;
}

MCL_INLINE bool MclStatus_IsNothingChanged(MclStatus status) {
	return status == MCL_STATUS_NOTHING_CHANGED;
}

///////////////////////////////////////////////////////////
#define MCL_FAILED(result)   MclStatus_IsFailed(result)
#define MCL_DONE(result)     MclStatus_IsDone(result)

MCL_STDC_END

#endif
