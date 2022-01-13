#include <cctest/cctest.h>
#include "mcl/thread/thread_launcher.h"
#include "mcl/array/array_size.h"
#include "mcl/lock/atom.h"

namespace {
	struct ThreadCtxt {
		size_t increaseCount;
		size_t decreaseCount;
		MclAtom result;
		MclAtom stop;
	};

	void increase(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		for (size_t i = 0; (i < threadCtxt->increaseCount); i++) {
			if (MclAtom_IsTrue(&threadCtxt->stop)) return;
			MclAtom_Add(&threadCtxt->result, 1);
			MclThread_Yield();
		}
	}

	void decrease(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		for (size_t i = 0; (i < threadCtxt->decreaseCount); i++) {
			if (MclAtom_IsTrue(&threadCtxt->stop)) return;
			MclAtom_Sub(&threadCtxt->result, 1);
			MclThread_Yield();
		}
	}

	void stop(void *ctxt) {
		ThreadCtxt *threadCtxt = (ThreadCtxt*)ctxt;
		MclAtom_Set(&threadCtxt->stop, 1);
	}
}

FIXTURE(ThreadLauncherTest) {
	TEST("should async launch threads") {
		ThreadCtxt ctxt = {.increaseCount = 100, .decreaseCount = 99, .result = 0, .stop = 0};

		MclThreadInfo threads[] = {
			MCL_THREAD_INFO("Increase", increase, NULL, &ctxt),
			MCL_THREAD_INFO("Dncrease", decrease, NULL, &ctxt),
		};

		MclThreadLauncher_Launch(threads, MCL_ARRAY_SIZE(threads));
		MclThreadLauncher_WaitDone(threads, MCL_ARRAY_SIZE(threads));

		ASSERT_EQ(1, ctxt.result);
	}

	TEST("should stop launched threads") {
		const uint32_t MAX_COUNT = MCL_UINT32_MAX;
		ThreadCtxt ctxt = {.increaseCount = MAX_COUNT, .decreaseCount = MAX_COUNT, .result = 0, .stop = 0};

		MclThreadInfo threads[] = {
			MCL_THREAD_INFO("Increase", increase, stop, &ctxt),
			MCL_THREAD_INFO("Dncrease", decrease, stop, &ctxt),
		};

		MclThreadLauncher_Launch(threads, MCL_ARRAY_SIZE(threads));
		MclThreadLauncher_WaitDone(threads, MCL_ARRAY_SIZE(threads));

		ASSERT_TRUE(ctxt.increaseCount < MAX_COUNT);
		ASSERT_TRUE(ctxt.decreaseCount < MAX_COUNT);
	}
};
