#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21B
#define MCL_F2A54BFBC3EF41079397B09166E3F21B

#include "mcl/status.h"
#include "mcl/typedef.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef void* MclListData;

/////////////////////////////////////////////////////////
MCL_INTERFACE(MclListDataPredIntf) {
    bool (*pred)(MclListDataPredIntf*, MclListData);
};

MCL_INLINE bool MclListDataPred_Predicate(MclListDataPredIntf *predIntf, MclListData data) {
    return (predIntf && predIntf->pred) ? predIntf->pred(predIntf, data) : false;
}

#define MCL_LIST_DATA_PRED_INTF(PRED) {.pred = PRED}

/////////////////////////////////////////////////////////
MCL_INTERFACE(MclListDataVisitIntf) {
    MclStatus (*visit)(MclListDataVisitIntf*, MclListData);
};

MCL_INLINE MclStatus MclListDataVisitor_Visit(MclListDataVisitIntf *visitIntf, MclListData data) {
    return (visitIntf && visitIntf->visit) ? visitIntf->visit(visitIntf, data) : MCL_FAILURE;
}

#define MCL_LIST_DATA_VISIT_INTF(VISIT) {.visit = VISIT}

/////////////////////////////////////////////////////////
//MCL_INTERFACE(MclListDataDestroyIntf) {
//    void (*destroy)(MclListDataDestroyIntf*, MclListData);
//};
//
//MCL_INLINE void MclListDataDestroyIntf_Destroy(MclListDataDestroyIntf *destroyIntf, MclListData data) {
//    if (destroyIntf && destroyIntf->destroy) destroyIntf->destroy(destroyIntf, data);
//}
//
//#define MCL_LIST_DATA_DESTROY_INTF(DESTROY) {.destroy = DESTROY}

typedef void (*MclListDataDestroy)(MclListData);

MCL_STDC_END

#endif
