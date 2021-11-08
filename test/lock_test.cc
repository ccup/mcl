#include <cctest/cctest.h>
#include "mcl/task/mutex.h"

FIXTURE(MutexTest)
{
	MclMutex mutex;

	BEFORE {
		MclMutex_InitRecursive(&mutex);
	}

	TEST("should not deal lock when double lock recursive mutex in same thread")
	{
		MclMutex_Lock(&mutex);
		MclMutex_Lock(&mutex);
		MclMutex_Unlock(&mutex);
		MclMutex_Unlock(&mutex);
	}
};
