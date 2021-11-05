#include "color.h"

const char* MclLogColor_GetStr(MclLogColor color) {
	switch (color) {
		case MCL_LOG_COLOR_RED :
			return "\033[1;31m";
		case MCL_LOG_COLOR_GREEN :
			return "\033[1;32m";
		case MCL_LOG_COLOR_YELLOW :
			return "\033[1;33m";
		case MCL_LOG_COLOR_BLUE :
			return "\033[1;34m";
		case MCL_LOG_COLOR_MAGENTA :
			return "\033[1;35m";
		case MCL_LOG_COLOR_CYAN :
			return "\033[1;36m";
		case MCL_LOG_COLOR_WHITE :
			return "\033[0m";
	}
	return "\033[0m";
}
