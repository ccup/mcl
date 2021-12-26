#ifndef H962ACD5A_FBFD_4782_80FB_F757B63CDFE2
#define H962ACD5A_FBFD_4782_80FB_F757B63CDFE2

#include "role/role_utils/energy.h"
#include "role/role_utils/human.h"

MCL_STDC_BEGIN

typedef struct HumanEnergy {
	MCL_ROLE_IMPL(Energy);
    MCL_ROLE_IMPL(Human);
}HumanEnergy;

void human_energy_init(HumanEnergy*, uint8_t);

MCL_STDC_END

#endif
