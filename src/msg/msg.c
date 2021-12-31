#include "mcl/msg/msg_queue.h"
#include "mcl/msg/msg.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MclStatus MclMsg_Init(MclMsg *self, MclMsgType type, MclMsgId id, uint16_t bodySize, void* body) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(body);
    MCL_ASSERT_TRUE(bodySize > 0);

    self->type = type;
    self->id = id;
    self->rev = 0;
    self->bodyBytes = bodySize;
    self->body = body;

    return MCL_SUCCESS;
}

MclMsg* MclMsg_Create(MclMsgType type, MclMsgId id, uint16_t bodySize) {
    MCL_ASSERT_TRUE_NIL(bodySize > 0);

    MclMsg *self = MCL_MALLOC(MclMsg_HeaderSize() + MclAlign_GetSizeOf(bodySize));
    MCL_ASSERT_VALID_PTR_NIL(self);

    if (MCL_FAILED(MclMsg_Init(self, type, id, bodySize, (uint8_t*)self + MclMsg_HeaderSize()))) {
        MCL_LOG_ERR("Init msg queue failed!");
        MCL_FREE(self);
        return NULL;
    }
    return self;
}

void MclMsg_Delete(MclMsg *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_FREE(self);
}

MclStatus MclMsg_Fill(MclMsg *self, uint16_t pos, uint16_t len, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(pos + len <= self->bodyBytes);

    memcpy(self->body, value, len);
    return MCL_SUCCESS;
}

MclStatus MclMsg_Fetch(MclMsg *self, uint16_t pos, uint16_t len, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(pos + len <= self->bodyBytes);

    memcpy(value, self->body, len);
    return MCL_SUCCESS;
}

MclStatus MclMsg_Copy(MclMsg *src, MclMsg *dst) {
    MCL_ASSERT_VALID_PTR(src);
    MCL_ASSERT_VALID_PTR(dst);
    MCL_ASSERT_VALID_PTR(src->body);
    MCL_ASSERT_VALID_PTR(dst->body);
    MCL_ASSERT_TRUE(src->bodyBytes >= dst->bodyBytes);

    dst->type = src->type;
    dst->id = src->id;
    dst->rev = src->rev;
    dst->bodyBytes = src->bodyBytes;
    if (dst->bodyBytes > 0) {
        memcpy(dst->body, src->body, dst->bodyBytes);
    }
    return MCL_SUCCESS;
}