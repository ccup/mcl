#ifndef H38247538_297F_4A80_94D3_8A289788461B
#define H38247538_297F_4A80_94D3_8A289788461B

#include "mcl/log/log_config.h"

///////////////////////////////////////////////////////////
#define MCL_LOG_TITLE(level, levelstr, fmt, ...) 		\
do {													\
	if (level & MCL_LOG_LEVELS) {						\
		MCL_LOG_OUTPUT(level, levelstr, __FILE__, __LINE__, fmt, ##__VA_ARGS__);\
	}													\
} while(0)

///////////////////////////////////////////////////////////
#define MCL_LOG_FATAL(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_FATAL, "FATAL", fmt, ##__VA_ARGS__)

#define MCL_LOG_ERR(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_ERR  , "ERROR", fmt, ##__VA_ARGS__)

#define MCL_LOG_WARN(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_WARN , "WARN" , fmt, ##__VA_ARGS__)

#define MCL_LOG_SUCC(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_SUCC , "SUCC" , fmt, ##__VA_ARGS__)

#define MCL_LOG_INFO(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_INFO , "INFO" , fmt, ##__VA_ARGS__)

#define MCL_LOG_DBG(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_DEBUG, "DEBUG", fmt, ##__VA_ARGS__)

#define MCL_LOG(fmt, ...) \
	MCL_LOG_TITLE(MCL_LOG_LEVEL_DEFAULT, "DEFAULT", fmt, ##__VA_ARGS__)

#endif
