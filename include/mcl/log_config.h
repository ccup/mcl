#ifndef H0198CF29_2964_442A_AF22_490F400D9756
#define H0198CF29_2964_442A_AF22_490F400D9756

#include "mcl/log_levels.h"

#ifdef CUSTOMER_LOG

#include "mcl/log_customized_config.h"

#else

#include <stdio.h>

#define MCL_LOG_PRINTF(level, levelstr, file, line, fmt, ...)\
	printf("[%s:0x%x] %s:%u: "#fmt"\n", levelstr, level, file, line, ##__VA_ARGS__)

#define MCL_LOG_OUTPUT MCL_LOG_PRINTF

#define MCL_LOG_LEVELS MCL_NONE_LEVEL

#endif

#endif
