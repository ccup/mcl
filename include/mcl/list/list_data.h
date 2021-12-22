#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21A
#define MCL_F2A54BFBC3EF41079397B09166E3F21A

#include "mcl/status.h"
#include "mcl/typedef.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef void* MclListData;

MCL_TYPE(MclListDataPred) {
    bool (*pred)(MclListDataPred*, MclListData data);
};

MCL_INLINE bool MclListDataPred_Predicate(MclListDataPred *pred, MclListData data) {
    return (pred && pred->pred) ? pred->pred(pred, data) : false;
}

#define MCL_LIST_DATA_PRED(PRED) {.pred = PRED}

MCL_TYPE(MclListDataVisitor) {
    MclStatus (*visit)(MclListDataVisitor*, MclListData);
};

MCL_INLINE MclStatus MclListDataVisitor_Visit(MclListDataVisitor *visitor, MclListData data) {
    return (visitor && visitor->visit) ? visitor->visit(visitor, data) : MCL_FAILURE;
}

#define MCL_LIST_DATA_VISITOR(VISITOR) {.visit = VISITOR}

MCL_TYPE(MclListDataDeleter) {
    void (*destroy)(MclListDataDeleter*, MclListData data);
};

MCL_INLINE void MclListDataDeleter_Destroy(MclListDataDeleter *deleter, MclListData data) {
    if (deleter && deleter->destroy) deleter->destroy(deleter, data);
}

#define MCL_LIST_DATA_DELETER(DESTROY) {.destroy = DESTROY}

MCL_STDC_END

#endif
