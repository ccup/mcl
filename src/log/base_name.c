#include "mcl/log/basename.h"
#include "mcl/stdtype.h"


const char* MclLog_GetBaseFile(const char* absPath) {
	if (!absPath) return "";

	const char* p = absPath + strlen(absPath);
	while((p != absPath) && ((*(p - 1) != '/') && (*(p - 1) != '\\'))) {
		p--;
	}
	return p;
}


