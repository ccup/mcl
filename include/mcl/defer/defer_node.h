#ifndef MCL_3547FA3A1CD64316A4CC149DA385CA1B
#define MCL_3547FA3A1CD64316A4CC149DA385CA1B

#include "mcl/typedef.h"
#include "mcl/keyword.h"
#include "mcl/link/link.h"

MCL_STDC_BEGIN

typedef void (*MclDeferFn)(void*);

MCL_TYPE(MclDeferNode) {
    MCL_LINK_NODE(MclDeferNode) link;
    MclDeferFn func;
    void* ctxt;
};

MCL_INLINE void MclDeferNode_Exec(MclDeferNode *self) {
    if (self && self->func) {
        self->func(self->ctxt);
    }
}

///////////////////////////////////////////////////////////
#define MCL_DEFER_NODE(FUNC, CTXT)      \
{.link = MCL_LINK_NODE_INITIALIZER(), .func = FUNC, .ctxt = CTXT}

MCL_STDC_END

#endif
