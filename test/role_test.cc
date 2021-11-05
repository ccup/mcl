#include <cctest/cctest.h>
#include "role/worker.h"
#include "role/energy.h"
#include "mcl/log.h"

FIXTURE(HumanTest) {
	Worker* worker{nullptr};
	BEFORE {
		worker = worker_create(HUMAN, 200);
	}

	AFTER {
		worker_release(worker);
	}

	TEST("human could produce") {
		worker_produce(worker);
		worker_produce(worker);
		ASSERT_EQ(2, worker->produce_num);
	}

	TEST("human could produce until exhausted") {
		Energy* energy = MCL_ROLE_OF(worker, Energy);
		while(!energy->exhausted(energy)) {
			worker_produce(worker);
		}
		ASSERT_EQ(20, worker->produce_num);
	}
};

FIXTURE(RobotTest) {
	Worker* worker{nullptr};
	BEFORE {
		worker = worker_create(ROBOT, 100);
	}

	AFTER {
		worker_release(worker);
	}

	TEST("robot could produce") {
		worker_produce(worker);
		worker_produce(worker);
		ASSERT_EQ(2, worker->produce_num);
	}

	TEST("robot could produce until exhausted") {
		Energy* energy = MCL_ROLE_OF(worker, Energy);
		while(!energy->exhausted(energy)) {
			worker_produce(worker);
		}
		ASSERT_EQ(10, worker->produce_num);
	}
};

FIXTURE(WorkerTest) {
	TEST("can only create two independent workers") {
		Worker* human = worker_create(HUMAN, 100);
		worker_produce(human);
		worker_produce(human);
		ASSERT_EQ(2, human->produce_num);

		Worker* robot = worker_create(ROBOT, 100);
		worker_produce(robot);
		ASSERT_EQ(1, robot->produce_num);

		Worker* unknown = worker_create(HUMAN, 100);
		ASSERT_EQ(NULL, unknown);

		worker_release(robot);

		unknown = worker_create(HUMAN, 100);
		ASSERT_NE(NULL, unknown);

		worker_produce(unknown);
		ASSERT_EQ(1, unknown->produce_num);

		worker_release(human);

		worker_produce(unknown);
		ASSERT_EQ(2, unknown->produce_num);

		worker_produce(unknown);
		ASSERT_EQ(3, unknown->produce_num);

		human = worker_create(HUMAN, 100);
		worker_produce(human);
		ASSERT_EQ(1, human->produce_num);
		worker_release(human);

		worker_produce(unknown);
		ASSERT_EQ(4, unknown->produce_num);
		worker_release(unknown);
	}
};
