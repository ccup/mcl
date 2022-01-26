#include "mcl/thread/thread_launcher.h"
#include "mcl/algo/loop.h"
#include "mcl/assert.h"

MCL_PRIVATE void MclThreadLauncher_WaitThread(MclThreadInfo *thread) {
	MCL_ASSERT_SUCC_CALL_VOID(MclThread_Join(thread->thread, NULL));
}

MCL_PRIVATE void MclThreadLauncher_StopThread(MclThreadInfo *thread) {
	if (thread->stop) thread->stop(thread->ctxt);
}

MCL_PRIVATE void* MclThreadLauncher_RunThread(void *data) {
	MclThreadInfo *thread = (MclThreadInfo*)data;

	MCL_LOG_DBG("Launch thread %s enter!", thread->name);

	MCL_ASSERT_SUCC_CALL_NIL(MclThread_SetName(thread->thread, thread->name));
	thread->run(thread->ctxt);

	MCL_LOG_DBG("Launch thread %s exit!", thread->name);
	return NULL;
}

MCL_PRIVATE MclStatus MclThreadLauncher_LaunchThread(MclThreadInfo *thread) {
	MCL_ASSERT_SUCC_CALL(MclThread_Create(&thread->thread, NULL, MclThreadLauncher_RunThread, thread));
	return MCL_SUCCESS;
}

MclStatus MclThreadLauncher_Launch(MclThreadInfo *threads, MclSize threadNum) {
	MCL_ASSERT_VALID_PTR(threads);

	MCL_LOOP_FOREACH_INDEX(i, threadNum) {
		MCL_ASSERT_SUCC_CALL(MclThreadLauncher_LaunchThread(&threads[i]));
	}
	return MCL_SUCCESS;
}

void MclThreadLauncher_WaitDone(MclThreadInfo  *threads, MclSize threadNum) {
	MCL_ASSERT_VALID_PTR_VOID(threads);

	MCL_LOOP_FOREACH_INDEX(i, threadNum) {
		MclThreadLauncher_StopThread(&threads[i]);
	}

	MCL_LOOP_FOREACH_INDEX(i, threadNum) {
		MclThreadLauncher_WaitThread(&threads[i]);
	}
}

