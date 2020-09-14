#ifndef H38247538_297F_4A80_94D3_8A289788461B
#define H38247538_297F_4A80_94D3_8A289788461B

#include "mcl/log_config.h"

#define __MCL_LOG_TITLE(level, levelstr, fmt, ...) 		\
do {													\
	if (level & MCL_LOG_LEVELS) {						\
		MCL_LOG_OUTPUT(level, levelstr, __FILE__, __LINE__, fmt, ##__VA_ARGS__);\
	}										\
} while(0)

#define MCL_FATAL(fmt, ...) \
	__MCL_LOG_TITLE(MCL_FATAL_LEVEL, "FATAL", fmt, ##__VA_ARGS__)

#define MCL_ERR(fmt, ...) \
	__MCL_LOG_TITLE(MCL_ERR_LEVEL  , "ERROR", fmt, ##__VA_ARGS__)

#define MCL_WARN(fmt, ...) \
	__MCL_LOG_TITLE(MCL_WARN_LEVEL , "WARN" , fmt, ##__VA_ARGS__)

#define MCL_SUCC(fmt, ...) \
	__MCL_LOG_TITLE(MCL_SUCC_LEVEL , "SUCC" , fmt, ##__VA_ARGS__)

#define MCL_INFO(fmt, ...) \
	__MCL_LOG_TITLE(MCL_INFO_LEVEL , "INFO" , fmt, ##__VA_ARGS__)

#define MCL_DBG(fmt, ...) \
	__MCL_LOG_TITLE(MCL_DEBUG_LEVEL, "DEBUG", fmt, ##__VA_ARGS__)

#endif
