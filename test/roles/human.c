#include "roles/human.h"

void human_init(Human* self, uint8_t energy) {
	self->is_hungry = false;
    self->consumed_energy = 0;
    self->total_energy = energy;
}

void human_eat_food(Human* self, uint8_t food) {
	self->total_energy += food;
	self->is_hungry = false;
}

void human_consume(Human* self, uint8_t energy) {
    self->consumed_energy += energy;

    if(self->consumed_energy >= self->total_energy)
    {
        self->is_hungry = true;
    }
}

bool human_exhaused(const Human* self) {
	return self->is_hungry;
}
