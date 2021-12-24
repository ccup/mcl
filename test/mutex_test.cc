#include <cctest/cctest.h>
#include "mcl/task/mutex.h"
#include "mcl/task/thread.h"

namespace {
	struct Obj {
		MclMutex mutex;
		int count{0};
	};

	void* increase(void *obj) {
		Obj* object = (Obj*)(obj);
		for (int i = 0; i < 10000; i++) {
			MCL_LOCK_SCOPE(object->mutex) {
				object->count++;
			}
		}
		return NULL;
	}

	void doDecrease(Obj& obj) {
		MCL_LOCK_AUTO(obj.mutex);
		obj.count--;
	}

	void* decrease(void *obj) {
		Obj* object = (Obj*)(obj);
		MCL_LOCK_SCOPE(object->mutex) {
			for (int i = 0; i < 10000; i++) {
				doDecrease(*object);
			}
		}
		return NULL;
	}

	void* decreaseInMiddle(void *obj) {
		Obj* object = (Obj*)(obj);
		MCL_LOCK_SCOPE(object->mutex) {
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
		MclMutex_InitRecursive(&obj.mutex);
	}

    TEST("should not dead lock when double lock recursive mutex in same thread")
    {
        MclMutex_Lock(&obj.mutex);
        MclMutex_Lock(&obj.mutex);
        MclMutex_Unlock(&obj.mutex);
        MclMutex_Unlock(&obj.mutex);
    }

	TEST("should auto lock")
	{
		MclThread t1, t2;
		MclThread_Create(&t1, NULL, increase, &obj);
		MclThread_Create(&t2, NULL, decrease, &obj);

		MCL_LOCK_SCOPE(obj.mutex) {
			obj.count += 2;
		}

		MclThread_Join(t1, NULL);
		MclThread_Join(t2, NULL);
		ASSERT_EQ(2, obj.count);
	}

	TEST("should unlock when return") {
		MclThread t1, t2;
		MclThread_Create(&t1, NULL, increase, &obj);
		MclThread_Create(&t2, NULL, decreaseInMiddle, &obj);

		MclThread_Join(t1, NULL);
		MclThread_Join(t2, NULL);
		ASSERT_EQ(5000, obj.count);
	}
};
