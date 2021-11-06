#ifndef HDD1B8843_1542_4D8A_ACBC_4D9719CA9B95
#define HDD1B8843_1542_4D8A_ACBC_4D9719CA9B95

#include "mcl/stdc.h"

#define MCL_TYPE_FWD(TYPE)  \
typedef struct TYPE TYPE

#define MCL_TYPE_DEF(TYPE)	\
MCL_TYPE_FWD(TYPE);			\
struct TYPE

#endif
