//#include "mcl/log/log_print.h"
//#include "mcl/log/log_base_name.h"
//#include <stdarg.h>
//#include <stdio.h>
//
//#define MCL_LOG_PRINTF(level, levelstr, file, line, fmt, ...)	\
//do {															\
//	if (level == MCL_LOG_LEVEL_DEFAULT) {						\
//		printf(fmt, ##__VA_ARGS__);								\
//	} else {													\
//		printf("%s", MclLogLevel_GetColorFmt(level));			\
//		printf("[%s:0x%x] %s:%u: " #fmt "\n", levelstr, level, MclLogBaseName_GetFile(file), line, ##__VA_ARGS__);\
//		printf("%s", MclLogLevel_GetColorFmt(MCL_LOG_LEVEL_NONE));\
//	}															\
//} while(0)
//
//
//void MclLog_Print(int level, const char* levelstr, const char* file, unsigned int line, const char* fmt, ...) {
//	printf("%s", MclLogLevel_GetColorFmt(level));
//	if (level == MCL_LOG_LEVEL_DEFAULT) {
//		printf(fmt, ...);
//	} else {
//
//	}
//	printf("%s", MclLogLevel_GetColorFmt(MCL_LOG_LEVEL_NONE));
//}
