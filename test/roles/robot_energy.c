#include "roles/robot_energy.h"
#include "mcl/keyword.h"

MCL_PRIVATE void robot_energy_consume(Energy* role, uint8_t energy) {
	ROLE_CAST_TO(Energy, role, RobotEnergy, self);
	robot_consume(SELF_ROLE_P(Robot), energy);
}

MCL_PRIVATE bool robot_energy_exhaused(const Energy* role) {
	ROLE_CAST_TO(Energy, role, RobotEnergy, self);
	return robot_exhaused(SELF_ROLE_P(Robot));
}

void robot_energy_init(RobotEnergy* self, uint8_t energy) {
	robot_init(SELF_ROLE_P(Robot), energy);
	SELF_ROLE_INIT_BY_TBL(Energy, {robot_energy_consume, robot_energy_exhaused});
}
