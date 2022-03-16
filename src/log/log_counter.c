#include "mcl/log/log_counter.h"
#include "mcl/lock/atomic.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtomic fatal;
	MclAtomic error;
	MclAtomic warn;
	MclAtomic others;
} MclLogCounter;

MCL_PRIVATE MclLogCounter counter = {0};

void MclLogCounter_CountLevel(MclLogLevel level) {
	switch (level) {
	case MCL_LOG_LEVEL_FATAL:
		MclAtomic_AddFetch(&counter.fatal, 1);
		break;
	case MCL_LOG_LEVEL_ERR:
		MclAtomic_AddFetch(&counter.error, 1);
		break;
	case MCL_LOG_LEVEL_WARN:
		MclAtomic_AddFetch(&counter.warn, 1);
		break;
	default:
		MclAtomic_AddFetch(&counter.others, 1);
		break;
	}
	return;
}

MclSize MclLogCounter_GetFatalCount() {
	return MclAtomic_Get(&counter.fatal);
}

MclSize MclLogCounter_GetErrorCount() {
	return MclAtomic_Get(&counter.error);
}

MclSize MclLogCounter_GetWarnCount() {
	return MclAtomic_Get(&counter.warn);
}

