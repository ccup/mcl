#include "mcl/log/log_printf.h"
#include "mcl/log/log_config.h"
#include "mcl/log/log_level.h"
#include "mcl/log/log_base_name.h"
#include "mcl/keyword.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef MCL_THREAD_ENABLED
#include "mcl/thread/thread.h"
#include "mcl/lock/mutex.h"

MCL_PRIVATE MclMutex mclLogMutex = MCL_MUTEX();

#define MCL_LOG_THREAD_NAME_LEN 16
#endif

#define MCL_LOG_STR_LEN_MAX    256

MCL_PRIVATE void MclLog_FormatPrintf(int level, const char* levelstr, const char* file, unsigned int line, const char* fmt, va_list args) {
		char str[MCL_LOG_STR_LEN_MAX] = {0};

        va_list argsCopy;
        va_copy(argsCopy, args);

        int length = vsnprintf(NULL, 0, fmt, argsCopy);
        va_end(argsCopy);

        if (length > MCL_LOG_STR_LEN_MAX) {
        	printf("[MclLog]: Drop log (%s:%u) because length (%u) exceeds max length (%u)\n", file, line, length, MCL_LOG_STR_LEN_MAX);
        	return;
        }
		va_copy(argsCopy, args);
		vsprintf(str, fmt, argsCopy);
		va_end(argsCopy);

		if (level == MCL_LOG_LEVEL_DEFAULT) {
			printf("%s", str);
		} else {
#ifdef MCL_THREAD_ENABLED
			MclThread tid = MclThread_GetId();
			char threadName[MCL_LOG_THREAD_NAME_LEN] = {0};
			MclThread_GetName(tid, threadName, MCL_LOG_THREAD_NAME_LEN);
			printf("[%s:0x%lx] [%s:0x%x] %s:%u: %s\n", threadName, (uintptr_t)tid, levelstr, level, file, line, str);
#else
			printf("[%s:0x%x] %s:%u: %s\n", levelstr, level, file, line, str);
#endif
		}
}

void MclLog_Printf(int level, const char* levelstr, const char* file, unsigned int line, const char* fmt, ...) {
	if ((level & (MCL_LOG_OUTPUT_LEVELS)) == 0) return;

#ifdef MCL_THREAD_ENABLED
	MCL_LOCK_AUTO(mclLogMutex);
#endif
	printf("%s", MclLogLevel_GetColorFmt(level));
    va_list va;
    va_start(va, fmt);
    MclLog_FormatPrintf(level, levelstr, MclLogBaseName_GetFile(file), line, fmt, va);
    va_end(va);
	printf("%s", MclLogLevel_GetColorFmt(MCL_LOG_LEVEL_NONE));
}
