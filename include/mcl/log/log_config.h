#ifndef H0198CF29_2964_442A_AF22_490F400D9756
#define H0198CF29_2964_442A_AF22_490F400D9756

#include "mcl/config.h"
#include "mcl/log/log_level.h"

#ifdef MCL_CONFIG_LOG

MCL_STDC_BEGIN

void MclLog_Output(int level , const char* levelstr , const char* file, unsigned int line, const char* fmt, ...);

MCL_STDC_END

#define MCL_LOG_OUTPUT(...)    MclLog_Output(__VA_ARGS__)

#else

#include "mcl/log/log_base_name.h"
#include <stdio.h>

#define MCL_LOG_PRINTF(level, levelstr, file, line, fmt, ...)	\
do {															\
	if (level == MCL_LOG_LEVEL_DEFAULT) {						\
		printf(fmt, ##__VA_ARGS__);								\
	} else {													\
		printf("%s", MclLogLevel_GetColorFmt(level));			\
		printf("[%s:0x%x] %s:%u: " #fmt "\n", levelstr, level, MclLog_GetBaseName(file), line, ##__VA_ARGS__);\
		printf("%s", MclLogLevel_GetColorFmt(MCL_LOG_LEVEL_NONE));\
	}															\
} while(0)

#define MCL_LOG_OUTPUT(...)    MCL_LOG_PRINTF(__VA_ARGS__)

#endif

/* Specify max logout level */
#define MCL_LOG_LEVELS MCL_LOG_LEVEL_TOTAL

#endif
