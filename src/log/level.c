#include "mcl/log/level.h"
#include "color.h"

const char* MclLogLevel_GetColorFmt(MclLogLevel level) {
	MclLogColor color;
	switch (level) {
	    case MCL_LOG_LEVEL_NONE  :
            color = MCL_LOG_COLOR_WHITE;
            break;
        case MCL_LOG_LEVEL_DEBUG :
            color = MCL_LOG_COLOR_BLUE;
            break;
	    case MCL_LOG_LEVEL_INFO  :
            color = MCL_LOG_COLOR_CYAN;
            break;
	    case MCL_LOG_LEVEL_SUCC  :
            color = MCL_LOG_COLOR_GREEN;
            break;
	    case MCL_LOG_LEVEL_WARN  :
            color = MCL_LOG_COLOR_MAGENTA;
            break;
	    case MCL_LOG_LEVEL_ERR   :
            color = MCL_LOG_COLOR_RED;
            break;
	    case MCL_LOG_LEVEL_FATAL :
            color = MCL_LOG_COLOR_RED;
            break;
        default:
            color = MCL_LOG_COLOR_WHITE;
	}
	return MclLogColor_GetStr(color);
}
