#include "roles/human_energy.h"
#include "mcl/keyword.h"

MCL_PRIVATE void human_energy_consume(Energy* role, uint8_t energy) {
	ROLE_CAST_TO(Energy, role, HumanEnergy, self);
	human_consume(SELF_ROLE_P(Human), energy);
}

MCL_PRIVATE bool human_energy_exhaused(const Energy* role) {
	ROLE_CAST_TO(Energy, role, HumanEnergy, self);
	return human_exhaused(SELF_ROLE_P(Human));
}

void human_energy_init(HumanEnergy* self, uint8_t energy) {
	human_init(SELF_ROLE_P(Human), energy);
	SELF_ROLE_INIT_BY_TBL(Energy, {human_energy_consume, human_energy_exhaused});
}
