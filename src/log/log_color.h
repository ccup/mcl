#ifndef HBB747DA6_8FA6_4BBB_83E4_6491B4949C29
#define HBB747DA6_8FA6_4BBB_83E4_6491B4949C29

#include "mcl/stdc.h"

MCL_STDC_BEGIN

typedef enum {
	MCL_LOG_COLOR_RED,
	MCL_LOG_COLOR_GREEN,
	MCL_LOG_COLOR_YELLOW,
	MCL_LOG_COLOR_BLUE,
	MCL_LOG_COLOR_MAGENTA,
	MCL_LOG_COLOR_CYAN,
	MCL_LOG_COLOR_WHITE,
} MclLogColor;

const char* MclLogColor_GetStr(MclLogColor);

MCL_STDC_END

#endif
