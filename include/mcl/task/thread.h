#ifndef H20F84EDA_8671_4E61_89B3_CD2C338853EB
#define H20F84EDA_8671_4E61_89B3_CD2C338853EB

#include "mcl/keyword.h"
#include "mcl/assert.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_t MclThread;
typedef pthread_attr_t MclThreadAttr;

MCL_INLINE MclStatus MclThread_Create(MclThread *thread, const MclThreadAttr *attr, void *(*func)(void*), void *args) {
	return pthread_create(thread, attr, func, args) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Detach(MclThread thread) {
	return pthread_detach(thread) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Join(MclThread thread, void **ext) {
	return pthread_join(thread, ext) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE void MclThread_Exit(MclThread *thread) {
	pthread_exit(thread);
}

MCL_INLINE void MclThread_Yield() {
	pthread_yield_np();
}

MCL_STDC_END

#endif
