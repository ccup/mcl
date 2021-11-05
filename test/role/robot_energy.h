#ifndef H0A114E99_3AA5_4618_8B6A_68471D72F571
#define H0A114E99_3AA5_4618_8B6A_68471D72F571

#include "role/energy.h"
#include "role/robot.h"

MCL_STDC_BEGIN

typedef struct RobotEnergy {
	MCL_ROLE_IMPL(Energy);
	MCL_ROLE_IMPL(Robot);
}RobotEnergy;

void robot_energy_init(RobotEnergy*, uint8_t);

MCL_STDC_END

#endif
