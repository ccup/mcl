#ifndef H0198CF29_2964_442A_AF22_490F400D9756
#define H0198CF29_2964_442A_AF22_490F400D9756

#include "mcl/log_levels.h"

#ifdef MCL_CUSTOMER_LOG

#include "mcl/stdc.h"

MCL_STDC_BEGIN

void MclLog_Customer(int level , const char* levelstr , const char* file, unsigned int line, const char* fmt, ...);

MCL_STDC_END

#define MCL_LOG_OUTPUT MclLog_Customer

#else

#include <stdio.h>

#define MCL_LOG_PRINTF(level, levelstr, file, line, fmt, ...)\
	printf("[%s:0x%x] %s:%u: "#fmt"\n", levelstr, level, file, line, ##__VA_ARGS__)

#define MCL_LOG_OUTPUT MCL_LOG_PRINTF

#endif

#define MCL_LOG_LEVELS MCL_TOTAL_LEVEL

#endif
