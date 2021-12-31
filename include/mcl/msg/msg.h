#ifndef MCL_0D7A2571016A48AEA8C02B9FFE7452F7
#define MCL_0D7A2571016A48AEA8C02B9FFE7452F7

#include "mcl/mem/align.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef uint16_t MclMsgType;
typedef uint16_t MclMsgId;

MCL_TYPE(MclMsg) {
    MclMsgType type;
    MclMsgId   id;
    uint16_t rev;
    uint16_t bodyBytes;
    void* body;
};

MclMsg* MclMsg_Create(MclMsgType type, MclMsgId, uint16_t bodySize);
void MclMsg_Delete(MclMsg*);

MclStatus MclMsg_Init(MclMsg*, MclMsgType, MclMsgId, uint16_t bodySize, void* body);

MclStatus MclMsg_Fill(MclMsg*, uint16_t pos, uint16_t len, void *value);
MclStatus MclMsg_Fetch(MclMsg *self, uint16_t pos, uint16_t len, void *value);
MclStatus MclMsg_Copy(MclMsg *src, MclMsg *dst);

MCL_INLINE void* MclMsg_GetBody(MclMsg *self) {
    return self ? self->body : NULL;
}

MCL_INLINE uint16_t MclMsg_GetBodySize(const MclMsg *self) {
    return self ? self->bodyBytes : 0;
}

MCL_INLINE uint16_t MclMsg_GetTotalSize(const MclMsg *self) {
    return self ? sizeof(MclMsg) + self->bodyBytes : 0;
}

MCL_INLINE size_t MclMsg_HeaderSize() {
    return MclAlign_GetSizeOf(sizeof(MclMsg));
}

///////////////////////////////////////////////////////////
MCL_INLINE void MclMsg_AutoFree(MclMsg **ppMsg) {
    if (!ppMsg) return;
    MclMsg_Delete(*ppMsg);
}

#define MCL_AUTO_MSG    MCL_RAII(MclMsg_AutoFree)

#define MCL_MSG(TYPE, ID, BODY_SIZE, BODY_PTR) \
{.type = TYPE, .id = ID, .rev = 0, .bodyBytes = BODY_SIZE, .body = BODY_PTR}

MCL_STDC_END

#endif
