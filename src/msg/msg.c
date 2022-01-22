#include "mcl/msg/msg_queue.h"
#include "mcl/msg/msg.h"
#include "mcl/mem/memory.h"
#include "mcl/assert.h"

MclStatus MclMsg_Init(MclMsg *self, MclMsgType type, MclMsgId id, MclSize bodySize, void* body) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(body);
    MCL_ASSERT_TRUE(bodySize > 0);

    self->type = type;
    self->id = id;
    self->sid = 0;
    self->bodyBytes = bodySize;
    self->body = body;

    return MCL_SUCCESS;
}

MclMsg* MclMsg_Create(MclMsgType type, MclMsgId id, MclSize bodySize) {
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

MclStatus MclMsg_Fill(MclMsg *self, MclSize pos, MclSize len, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(pos + len <= self->bodyBytes);

    MCL_MEM_COPY(self->body, value, len);
    return MCL_SUCCESS;
}

MclStatus MclMsg_Fetch(MclMsg *self, MclSize pos, MclSize len, void *value) {
    MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_VALID_PTR(value);
    MCL_ASSERT_TRUE(pos + len <= self->bodyBytes);

    MCL_MEM_COPY(value, self->body, len);
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
    dst->sid = src->sid;
    dst->bodyBytes = src->bodyBytes;
    if (dst->bodyBytes > 0) {
    	MCL_MEM_COPY(dst->body, src->body, dst->bodyBytes);
    }
    return MCL_SUCCESS;
}
