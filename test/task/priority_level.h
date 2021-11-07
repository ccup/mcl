#ifndef H86DDFF0F_A4BB_447D_8639_678025E56AAE
#define H86DDFF0F_A4BB_447D_8639_678025E56AAE

#include <string>

enum PriorityLevel {
	URGENT, NORMAL, SLOW, MAX_PRIORITY
};

std::string PriorityLevel_GetStr(PriorityLevel priority) {
	switch (priority) {
	case URGENT : return "URGENT";
	case NORMAL : return "NORMAL";
	case SLOW : return "SLOW";
	default : break;
	}
	return "UNKNOWN";
}

#endif
