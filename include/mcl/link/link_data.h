#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21A
#define MCL_F2A54BFBC3EF41079397B09166E3F21A

#include "mcl/status.h"
#include "mcl/typedef.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef void* MclLinkData;

MCL_TYPE_DEF(MclLinkDataPred) {
    bool (*pred)(MclLinkDataPred*, MclLinkData data);
};

MCL_INLINE bool MclLinkDataPred_Predicate(MclLinkDataPred *pred, MclLinkData data) {
    return (pred && pred->pred) ? pred->pred(pred, data) : false;
}

#define MCL_LINK_DATA_PRED(PRED) {.pred = PRED}

MCL_TYPE_DEF(MclLinkDataVisitor) {
    MclStatus (*visit)(MclLinkDataVisitor*, MclLinkData);
};

MCL_INLINE MclStatus MclLinkDataVisitor_Visit(MclLinkDataVisitor *visitor, MclLinkData data) {
    return (visitor && visitor->visit) ? visitor->visit(visitor, data) : MCL_FAILURE;
}

#define MCL_LINK_DATA_VISITOR(VISITOR) {.visit = VISITOR}

MCL_TYPE_DEF(MclLinkDataDeleter) {
    void (*destroy)(MclLinkDataDeleter*, MclLinkData data);
};

MCL_INLINE void MclLinkDataDeleter_Destroy(MclLinkDataDeleter *deleter, MclLinkData data) {
    if (deleter && deleter->destroy) deleter->destroy(deleter, data);
}

#define MCL_LINK_DATA_DELETER(DESTROY) {.destroy = DESTROY}

MCL_STDC_END

#endif
