#ifndef HA25033D6_1564_4748_B2C8_4DE2C5A286DE
#define HA25033D6_1564_4748_B2C8_4DE2C5A286DE

#include "mcl/stdc.h"
#include <stdbool.h>
#include <stdint.h>

MCL_STDC_BEGIN

typedef uint32_t MclStatus;

#define MCL_SUCC_STATUS(status) (MclStatus) status
#define MCL_FAIL_STATUS(status) (MclStatus) (status | MCL_RESERVED_FAIL)


/* OK */
#define MCL_SUCCESS             MCL_SUCC_STATUS(0)

/* Error Status */
#define MCL_RESERVED_FAIL       (MclStatus) 0x80000000
#define MCL_FAILURE             MCL_FAIL_STATUS(1)
#define MCL_FATAL_BUG           MCL_FAIL_STATUS(2)
#define MCL_TIMEDOUT            MCL_FAIL_STATUS(3)
#define MCL_OUT_OF_RANGE        MCL_FAIL_STATUS(4)
#define MCL_UNIMPLEMENTED       MCL_FAIL_STATUS(5)

static inline bool mcl_status_is_ok(MclStatus status)
{
    return (status & MCL_RESERVED_FAIL) == 0;
}

static inline bool mcl_status_is_fail(MclStatus status)
{
    return !mcl_status_is_ok(status);
}

#define __MCL_FAILED(result)   mcl_status_is_fail(result)

MCL_STDC_END

#endif
