#include "role/human_energy.h"
#include "mcl/keyword.h"

MCL_PRIVATE void human_energy_consume(Energy* role, uint8_t energy) {
	MCL_ROLE_CAST_TO(Energy, role, HumanEnergy, self);
	human_consume(MCL_ROLE_SELF_P(Human), energy);
}

MCL_PRIVATE bool human_energy_exhaused(const Energy* role) {
	MCL_ROLE_CAST_TO(Energy, role, HumanEnergy, self);
	return human_exhaused(MCL_ROLE_SELF_P(Human));
}

void human_energy_init(HumanEnergy* self, uint8_t energy) {
	human_init(MCL_ROLE_SELF_P(Human), energy);
	MCL_ROLE_INIT_SELF_BY_TBL(Energy, {human_energy_consume, human_energy_exhaused});
}
