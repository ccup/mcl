#include "mcl/defer/defer.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclDefer_Reset(MclDefer *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_LINK_INIT(&self->defers, MclDeferNode, link);
}

void MclDefer_Init(MclDefer *self) {
    MclDefer_Reset(self);
}

void MclDefer_Clear(MclDefer *self) {
    MclDefer_Reset(self);
}

void MclDefer_Add(MclDefer *self, MclDeferNode *defer) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_VALID_PTR_VOID(defer);
    MCL_LINK_INSERT_HEAD(&self->defers, defer, MclDeferNode, link);
}

void MclDefer_Exec(MclDefer *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);

    MclDeferNode *node = NULL;
    MCL_LINK_FOREACH(&self->defers, MclDeferNode, link, node) {
        MclDeferNode_Exec(node);
    }
}
