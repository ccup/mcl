#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21B
#define MCL_F2A54BFBC3EF41079397B09166E3F21B

#include "mcl/status.h"

MCL_STDC_BEGIN

typedef void* MclListData;

typedef void (*MclListDataDestroy)(MclListData);
typedef bool (*MclListDataPred)(MclListData, void*);
typedef MclStatus (*MclListDataVisit)(MclListData, void*);

MCL_STDC_END

#endif
