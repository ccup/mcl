#include "role/robot_energy.h"
#include "mcl/keyword.h"

MCL_PRIVATE void robot_energy_consume(Energy* role, uint8_t energy) {
	MCL_ROLE_CAST_TO(Energy, role, RobotEnergy, self);
	robot_consume(MCL_ROLE_SELF_P(Robot), energy);
}

MCL_PRIVATE bool robot_energy_exhaused(const Energy* role) {
	MCL_ROLE_CAST_TO(Energy, role, RobotEnergy, self);
	return robot_exhaused(MCL_ROLE_SELF_P(Robot));
}

void robot_energy_init(RobotEnergy* self, uint8_t energy) {
	robot_init(MCL_ROLE_SELF_P(Robot), energy);
	MCL_ROLE_INIT_SELF_BY_TBL(Energy, {robot_energy_consume, robot_energy_exhaused});
}
