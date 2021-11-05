#ifndef H0198CF29_2964_442A_AF22_490F400D9756
#define H0198CF29_2964_442A_AF22_490F400D9756

#include "mcl/log/level.h"

#ifdef MCL_CUSTOMER_LOG

extern "C" void MclLog_Customer(int level , const char* levelstr , const char* file, unsigned int line, const char* fmt, ...);

#define MCL_LOG_OUTPUT MclLog_Customer

#else

#include "mcl/log/basename.h"
#include <stdio.h>

#define MCL_LOG_PRINTF(level, levelstr, file, line, fmt, ...)	\
do {															\
	printf("%s", MclLogLevel_GetColorFmt(level));			 	\
	printf("[%s:0x%x] %s:%u: " #fmt "\n", levelstr, level, MclLog_GetBaseFile(file), line, ##__VA_ARGS__);\
	printf("%s", MclLogLevel_GetColorFmt(MCL_LOG_LEVEL_NONE));	\
} while(0)

#define MCL_LOG_OUTPUT MCL_LOG_PRINTF

#endif

#define MCL_LOG_LEVELS MCL_LOG_LEVEL_TOTAL

#endif
