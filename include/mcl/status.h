#ifndef HA25033D6_1564_4748_B2C8_4DE2C5A286DE
#define HA25033D6_1564_4748_B2C8_4DE2C5A286DE

#include "mcl/stdc.h"
#include <stdbool.h>
#include <stdint.h>

MCL_STDC_BEGIN

typedef uint32_t MclStatus;

#define MCL_STATUS_SUCC(status) (MclStatus) status
#define MCL_STATUS_FAIL(status) (MclStatus) (status | MCL_RESERVED_FAIL)

/* OK */
#define MCL_SUCCESS             MCL_STATUS_SUCC(0)

/* Error Status */
#define MCL_RESERVED_FAIL       (MclStatus) 0x80000000
#define MCL_FAILURE             MCL_STATUS_FAIL(1)
#define MCL_FATAL_BUG           MCL_STATUS_FAIL(2)
#define MCL_TIMEDOUT            MCL_STATUS_FAIL(3)
#define MCL_OUT_OF_RANGE        MCL_STATUS_FAIL(4)
#define MCL_UNIMPLEMENTED       MCL_STATUS_FAIL(5)

static inline bool MclStatus_IsOK(MclStatus status)
{
    return (status & MCL_RESERVED_FAIL) == 0;
}

static inline bool MclStatus_IsFailed(MclStatus status)
{
    return !MclStatus_IsOK(status);
}

#define MCL_FAILED(result)   MclStatus_IsFailed(result)

MCL_STDC_END

#endif
