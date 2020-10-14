#include "role/worker.h"
#include "role/robot_energy.h"
#include "role/human_energy.h"
#include "mcl/allocator.h"

typedef struct WorkerObject {
	IMPL_ROLE(Worker);
	union EnergyMem{
		RobotEnergy robot;
		HumanEnergy human;
	}energy_mem;
} WorkerObject;

MCL_DEF_ALLOCATOR(WorkerObject, 2);

MCL_PRIVATE void worker_init(Worker* self, Energy *role) {
	self->produce_num = 0;
	SELF_ROLE(Energy) = role;
}

MCL_PRIVATE Energy* worker_object_create_energy(WorkerObject* self, WorkerType type, uint8_t energy) {
	switch(type) {
	case HUMAN:
		human_energy_init(&self->energy_mem.human, energy);
		return &self->energy_mem.human.ROLE(Energy);
	case ROBOT:
		robot_energy_init(&self->energy_mem.robot, energy);
		return &self->energy_mem.robot.ROLE(Energy);
	default:
		break;
	}
	return NULL;
}

MCL_PRIVATE MclStatus worker_object_init(WorkerObject* self, WorkerType type, uint8_t energy) {
	Energy* energy_role = worker_object_create_energy(self, type, energy);
	if (energy_role == NULL) return MCL_FAILURE;
	worker_init(SELF_ROLE_P(Worker), energy_role);
	return MCL_SUCCESS;
}

Worker* worker_create(WorkerType type, uint8_t energy) {
	WorkerObject* self = MCL_ALLOCATOR(WorkerObject).alloc();
	if (self == NULL) return NULL;
	MCL_ASSERT_SUCC_CALL_NIL(worker_object_init(self, type, energy));
	return SELF_ROLE_P(Worker);
}

void worker_release(Worker* self) {
	ROLE_CAST_TO(Worker, self, WorkerObject, obj);
	MCL_ALLOCATOR(WorkerObject).free(obj);
}

void worker_produce(Worker* self) {
	if(SELF_ROLE(Energy)->exhausted(SELF_ROLE(Energy))) return;

    self->produce_num++;

    SELF_ROLE(Energy)->consume(SELF_ROLE(Energy), 10);
}
