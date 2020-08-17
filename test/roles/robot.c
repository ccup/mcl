#include "roles/robot.h"

void robot_init(Robot* self, uint8_t percent) {
	self->charge_percent = percent;
}

void robot_charge(Robot* self, uint8_t percent) {
	self->charge_percent += percent;
	if (self->charge_percent > 100) self->charge_percent = 100;
}

void robot_consume(Robot* self, uint8_t energy) {
	self->charge_percent -= energy;
}

bool robot_exhaused(const Robot* self) {
	return self->charge_percent <= 0;
}
