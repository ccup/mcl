#ifndef H6321B778_C5D0_4722_A771_3DFA00560B07
#define H6321B778_C5D0_4722_A771_3DFA00560B07

#include "mcl/thread/thread.h"

MCL_STDC_BEGIN

MCL_TYPE(MclThreadInfo) {
	MclThread thread;
	const char *name;
	void (*run)(void *ctxt);
	void (*stop)(void *ctxt);
	void *ctxt;
};

MclStatus MclThreadLauncher_Launch(MclThreadInfo*, size_t threadNum);
void MclThreadLauncher_WaitDone(MclThreadInfo*, size_t threadNum);

///////////////////////////////////////////////////////////
#define MCL_THREAD_INFO(NAME, RUN, STOP, CTXT)				\
{.name = (NAME), .run = (RUN), .stop = (STOP), .ctxt = (CTXT)}

MCL_STDC_END

#endif
