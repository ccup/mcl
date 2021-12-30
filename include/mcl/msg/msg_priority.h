#ifndef MCL_129EB2106E3F47B98AAF2DE052B8476F
#define MCL_129EB2106E3F47B98AAF2DE052B8476F

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef enum {
    MCL_MSG_URGENT,
    MCL_MSG_NORMAL,
} MclMsgPriority;

MCL_INLINE bool MclMsgPriority_IsValid(MclMsgPriority self) {
    return self <= MCL_MSG_NORMAL;
}

MCL_STDC_END

#endif
