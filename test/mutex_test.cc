#include <cctest/cctest.h>
#include "mcl/lock.h"

namespace {
	struct Obj {
		MclMutex mutex;
		int count{0};
	};

	void* increase(void *obj) {
		Obj* object = (Obj*)(obj);
		for (int i = 0; i < 10000; i++) {
			MCL_SCOPE_LOCK(object->mutex) {
				object->count++;
			}
		}
		return NULL;
	}

	void doDecrease(Obj& obj) {
		MCL_AUTO_LOCK(obj.mutex);
		obj.count--;
	}

	void* decrease(void *obj) {
		Obj* object = (Obj*)(obj);
		MCL_SCOPE_LOCK(object->mutex) {
			for (int i = 0; i < 10000; i++) {
				doDecrease(*object);
			}
		}
		return NULL;
	}

	void* decreaseInMiddle(void *obj) {
		Obj* object = (Obj*)(obj);
		MCL_SCOPE_LOCK(object->mutex) {
			for (int i = 0; i < 10000; i++) {
				if (i >= 5000) return NULL;
				object->count--;
			}
		}
		return NULL;

	}
}

FIXTURE(MutexTest)
{
	Obj obj;

	BEFORE {
		Mcl_InitRecursiveMutex(&obj.mutex);
	}

	TEST("should auto lock")
	{
		pthread_t t1, t2;
		pthread_create(&t1, NULL, increase, &obj);
		pthread_create(&t2, NULL, decrease, &obj);

		MCL_SCOPE_LOCK(obj.mutex) {
			obj.count += 2;
		}

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		ASSERT_EQ(2, obj.count);
	}

	TEST("should unlock when return") {
		pthread_t t1, t2;
		pthread_create(&t1, NULL, increase, &obj);
		pthread_create(&t2, NULL, decreaseInMiddle, &obj);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		ASSERT_EQ(5000, obj.count);
	}
};