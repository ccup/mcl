#ifndef H962ACD5A_FBFD_4782_80FB_F757B63CDFE2
#define H962ACD5A_FBFD_4782_80FB_F757B63CDFE2

#include "role/energy.h"
#include "role/human.h"

MCL_STDC_BEGIN

typedef struct HumanEnergy {
	IMPL_ROLE(Energy);
    IMPL_ROLE(Human);
}HumanEnergy;

void human_energy_init(HumanEnergy*, uint8_t);

MCL_STDC_END

#endif
