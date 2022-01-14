#ifndef MCL_0D7A2571016A48AEA8C02B9FFE7452F7
#define MCL_0D7A2571016A48AEA8C02B9FFE7452F7

#include "mcl/mem/align.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef uint32_t MclMsgType;
typedef uint16_t MclMsgId;
typedef uint16_t MclMsgSessionId;

MCL_TYPE(MclMsg) {
    MclMsgType type;
    MclMsgId   id;
    MclMsgSessionId sid;
    MclSize bodyBytes;
    void* body;
};

MclMsg* MclMsg_Create(MclMsgType type, MclMsgId, MclSize bodySize);
void MclMsg_Delete(MclMsg*);

MclStatus MclMsg_Init(MclMsg*, MclMsgType, MclMsgId, MclSize bodySize, void* body);

MclStatus MclMsg_Fill(MclMsg*, MclSize pos, MclSize len, void *value);
MclStatus MclMsg_Fetch(MclMsg *self, MclSize pos, MclSize len, void *value);
MclStatus MclMsg_Copy(MclMsg *src, MclMsg *dst);

MCL_INLINE void* MclMsg_GetBody(MclMsg *self) {
    return self ? self->body : NULL;
}

MCL_INLINE MclSize MclMsg_GetBodySize(const MclMsg *self) {
    return self ? self->bodyBytes : 0;
}

MCL_INLINE MclSize MclMsg_GetTotalSize(const MclMsg *self) {
    return self ? sizeof(MclMsg) + self->bodyBytes : 0;
}

MCL_INLINE MclSize MclMsg_HeaderSize() {
    return MclAlign_GetSizeOf(sizeof(MclMsg));
}

///////////////////////////////////////////////////////////
MCL_INLINE void MclMsg_AutoFree(MclMsg **ppMsg) {
    if (!ppMsg) return;
    MclMsg_Delete(*ppMsg);
}

#define MCL_AUTO_MSG    MCL_RAII(MclMsg_AutoFree)

#define MCL_MSG(TYPE, ID, BODY_SIZE, BODY_PTR) \
{.type = TYPE, .id = ID, .sid = 0, .bodyBytes = BODY_SIZE, .body = BODY_PTR}

MCL_STDC_END

#endif
