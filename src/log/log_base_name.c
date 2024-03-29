#include "mcl/log/log_base_name.h"
#include "mcl/stdtype.h"

const char* MclLogBaseName_GetFile(const char* absPath) {
	if (!absPath) return "";

	const char* p = absPath + strlen(absPath);
	while((p != absPath) && ((*(p - 1) != '/') && (*(p - 1) != '\\'))) {
		p--;
	}
	return p;
}


