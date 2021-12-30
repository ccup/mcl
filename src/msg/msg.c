#include "mcl/msg/msg.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MclMsg* MclMsg_Create(uint16_t bodySize) {
    MCL_ASSERT_TRUE_NIL(bodySize > 0);

    MclMsg *self = MCL_MALLOC(sizeof(MclMsg) + bodySize);
    MCL_ASSERT_VALID_PTR_NIL(self);

    return self;
}

void MclMsg_Delete(MclMsg *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_FREE(self);
}
