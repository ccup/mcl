#ifndef H56BA330C_3488_4252_9784_B705C2E4D6F3
#define H56BA330C_3488_4252_9784_B705C2E4D6F3

#include "mcl/role.h"
#include <stdint.h>
#include <stdbool.h>

MCL_STDC_BEGIN

MCL_ROLE_DEF(Energy) {
	void (*consume)(Energy*, uint8_t);
	bool (*exhausted)(const Energy*);
};

MCL_STDC_END

#endif
