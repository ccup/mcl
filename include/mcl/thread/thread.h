#ifndef H20F84EDA_8671_4E61_89B3_CD2C338853EB
#define H20F84EDA_8671_4E61_89B3_CD2C338853EB

#include "mcl/keyword.h"
#include "mcl/assert.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_t MclThread;
typedef pthread_attr_t MclThreadAttr;

MCL_INLINE MclStatus MclThread_Create(MclThread *thread, const MclThreadAttr *attr, void *(*func)(void*), void *args) {
    int ret = pthread_create(thread, attr, func, args);
    if (ret) {
        MCL_LOG_ERR("pthread_create fail %d!", ret);
    }
	return ret ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Detach(MclThread thread) {
	return pthread_detach(thread) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Join(MclThread thread, void **ext) {
    int ret = pthread_join(thread, ext);
    if (ret) {
        MCL_LOG_ERR("pthread_join fail %d!", ret);
    }
    return ret ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE void MclThread_Exit(MclThread *thread) {
	pthread_exit(thread);
}

MCL_INLINE void MclThread_Yield() {
    sched_yield();
}

MCL_INLINE uintptr_t MclThread_GetId() {
    return (uintptr_t)pthread_self();
}

MCL_INLINE MclStatus MclThread_SetName(MclThread thread, const char *name) {
#ifdef MCL_OS_MAC
	return pthread_setname_np(name) ? MCL_FAILURE : MCL_SUCCESS;
#else
    return pthread_setname_np(thread, name) ? MCL_FAILURE : MCL_SUCCESS;
#endif
}

MCL_INLINE MclStatus MclThread_GetName(MclThread thread, char *name, size_t len) {
    return pthread_getname_np(thread, name, len) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_STDC_END

#endif
