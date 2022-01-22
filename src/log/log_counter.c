#include "mcl/log/log_counter.h"
#include "mcl/lock/atom.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtom fatal;
	MclAtom error;
	MclAtom warn;
	MclAtom others;
} MclLogCounter;

MCL_PRIVATE MclLogCounter counter = {0};

void MclLogCounter_CountLevel(MclLogLevel level) {
	switch (level) {
	case MCL_LOG_LEVEL_FATAL:
		MclAtom_AddFetch(&counter.fatal, 1);
		break;
	case MCL_LOG_LEVEL_ERR:
		MclAtom_AddFetch(&counter.error, 1);
		break;
	case MCL_LOG_LEVEL_WARN:
		MclAtom_AddFetch(&counter.warn, 1);
		break;
	default:
		MclAtom_AddFetch(&counter.others, 1);
		break;
	}
	return;
}

MclSize MclLogCounter_GetFatalCount() {
	return MclAtom_Get(&counter.fatal);
}

MclSize MclLogCounter_GetErrorCount() {
	return MclAtom_Get(&counter.error);
}

MclSize MclLogCounter_GetWarnCount() {
	return MclAtom_Get(&counter.warn);
}

