#ifndef MCL_LINK_DATA_H
#define MCL_LINK_DATA_H

#include "mcl/stdc.h"

MCL_STDC_BEGIN

typedef void* MclLinkData;

typedef void (*MclLinkDataDeleter)(MclLinkData);

MCL_STDC_END

#endif
