#include <cctest/cctest.h>
#include "mcl/mutex.h"

namespace {
	struct Obj {
		MCL_MUTEX mutex;
		int count{0};
	};

	void* increase(void *obj) {
		Obj* object = (Obj*)(obj);
		for (int i = 0; i < 10000; i++) {
			Mcl_LockMutex(&object->mutex);
			object->count++;
			Mcl_UnlockMutex(&object->mutex);
		}
		return NULL;
	}

	void doDecrease(Obj& obj) {
		Mcl_LockMutex(&obj.mutex);
		obj.count--;
		Mcl_UnlockMutex(&obj.mutex);
	}

	void* decrease(void *obj) {
		Obj* object = (Obj*)(obj);
		Mcl_LockMutex(&object->mutex);
		for (int i = 0; i < 10000; i++) {
			doDecrease(*object);
		}
		Mcl_UnlockMutex(&object->mutex);
		return NULL;
	}
}

FIXTURE(MutexTest)
{
	Obj obj;

	BEFORE {
		Mcl_InitRecursiveMutex(&obj.mutex);
	}

	TEST("should lock threads")
	{
		pthread_t t1, t2;
		pthread_create(&t1, NULL, increase, &obj);
		pthread_create(&t2, NULL, decrease, &obj);

		Mcl_LockMutex(&obj.mutex);
		obj.count += 2;
		Mcl_UnlockMutex(&obj.mutex);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		ASSERT_EQ(2, obj.count);
	}
};