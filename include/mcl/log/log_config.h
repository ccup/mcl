#ifndef H0198CF29_2964_442A_AF22_490F400D9756
#define H0198CF29_2964_442A_AF22_490F400D9756

#include "mcl/config.h"
#include "mcl/log/log_level.h"

#ifdef MCL_CONFIG_LOG

MCL_STDC_BEGIN

void MclLog_Output(int level, const char* levelstr, const char* file, unsigned int line, const char* fmt, ...);

MCL_STDC_END

#define MCL_LOG_OUTPUT(...)        MclLog_Output(__VA_ARGS__)

#else

#include "mcl/log/log_printf.h"

#define MCL_LOG_OUTPUT(...)        MclLog_Printf(__VA_ARGS__)

/* Specify log output levels */
#define MCL_LOG_OUTPUT_LEVELS      MCL_LOG_LEVEL_TOTAL

#endif

#endif
