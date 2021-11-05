#include "role/worker.h"
#include "role/robot_energy.h"
#include "role/human_energy.h"
#include "mcl/allocator.h"

typedef struct WorkerObject {
	MCL_ROLE_IMPL(Worker);
	union EnergyMem{
		RobotEnergy robot;
		HumanEnergy human;
	}energy_mem;
} WorkerObject;

MCL_ALLOCATOR_DEF(WorkerObject, 2);

MCL_PRIVATE void worker_init(Worker* self, Energy *role) {
	self->produce_num = 0;
	MCL_ROLE_SELF(Energy) = role;
}

MCL_PRIVATE Energy* worker_object_create_energy(WorkerObject* self, WorkerType type, uint8_t energy) {
	switch(type) {
	case HUMAN:
		human_energy_init(&self->energy_mem.human, energy);
		return &self->energy_mem.human.MCL_ROLE(Energy);
	case ROBOT:
		robot_energy_init(&self->energy_mem.robot, energy);
		return &self->energy_mem.robot.MCL_ROLE(Energy);
	default:
		break;
	}
	return NULL;
}

MCL_PRIVATE MclStatus worker_object_init(WorkerObject* self, WorkerType type, uint8_t energy) {
	Energy* energy_role = worker_object_create_energy(self, type, energy);
	if (energy_role == NULL) return MCL_FAILURE;
	worker_init(MCL_ROLE_SELF_P(Worker), energy_role);
	return MCL_SUCCESS;
}

Worker* worker_create(WorkerType type, uint8_t energy) {
	WorkerObject* self = MCL_ALLOCATOR(WorkerObject).alloc();
	if (self == NULL) return NULL;
	MCL_ASSERT_SUCC_CALL_NIL(worker_object_init(self, type, energy));
	return MCL_ROLE_SELF_P(Worker);
}

void worker_release(Worker* self) {
	MCL_ROLE_CAST_TO(Worker, self, WorkerObject, obj);
	MCL_ALLOCATOR(WorkerObject).free(obj);
}

void worker_produce(Worker* self) {
	if(MCL_ROLE_SELF(Energy)->exhausted(MCL_ROLE_SELF(Energy))) return;

    self->produce_num++;

    MCL_ROLE_SELF(Energy)->consume(MCL_ROLE_SELF(Energy), 10);
}
