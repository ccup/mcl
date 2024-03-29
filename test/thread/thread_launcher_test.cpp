#include <cctest/cctest.h>
#include "mcl/thread/thread_launcher.h"
#include "mcl/array/array_size.h"
#include "mcl/lock/atomic.h"

namespace {
	struct ThreadCtxt {
		MclSize increaseMax;
		MclSize decreaseMax;
		MclSize increaseCount;
		MclSize decreaseCount;
		MclAtomic result;
		MclAtomic stop;
	};

	void increase(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		for (MclSize i = 0; (i < threadCtxt->increaseMax); i++) {
			if (MclAtomic_IsTrue(&threadCtxt->stop)) return;
			MclAtomic_AddFetch(&threadCtxt->result, 1);
			threadCtxt->increaseCount++;
			MclThread_Yield();
		}
	}

	void decrease(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		for (MclSize i = 0; (i < threadCtxt->decreaseMax); i++) {
			if (MclAtomic_IsTrue(&threadCtxt->stop)) return;
			MclAtomic_SubFetch(&threadCtxt->result, 1);
			threadCtxt->decreaseCount++;
			MclThread_Yield();
		}
	}

	void stop(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		MclAtomic_Set(&threadCtxt->stop, 1);
	}
}

FIXTURE(ThreadLauncherTest) {
	TEST("should async launch threads") {
		ThreadCtxt ctxt = {.increaseMax = 100, .decreaseMax = 99, .result = 0, .stop = 0};

		MclThreadInfo threads[] = {
			MCL_THREAD_INFO("Increase", increase, NULL, &ctxt),
			MCL_THREAD_INFO("Decrease", decrease, NULL, &ctxt),
		};

		MclThreadLauncher_Launch(threads, MCL_ARRAY_SIZE(threads));
		MclThreadLauncher_WaitDone(threads, MCL_ARRAY_SIZE(threads));

		ASSERT_EQ(100, ctxt.increaseCount);
		ASSERT_EQ(99, ctxt.decreaseCount);
		ASSERT_EQ(1, ctxt.result);
	}

	TEST("should stop launched threads") {
		const MclSize MAX_COUNT = MCL_SIZE_MAX;
		ThreadCtxt ctxt = {.increaseMax = MAX_COUNT, .decreaseMax = MAX_COUNT, .result = 0, .stop = 0};

		MclThreadInfo threads[] = {
			MCL_THREAD_INFO("Increase", increase, stop, &ctxt),
			MCL_THREAD_INFO("Decrease", decrease, stop, &ctxt),
		};

		MclThreadLauncher_Launch(threads, MCL_ARRAY_SIZE(threads));
		MclThreadLauncher_WaitDone(threads, MCL_ARRAY_SIZE(threads));

		ASSERT_TRUE(ctxt.increaseCount < MAX_COUNT);
		ASSERT_TRUE(ctxt.decreaseCount < MAX_COUNT);
	}
};
