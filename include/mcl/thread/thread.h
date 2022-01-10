#ifndef H20F84EDA_8671_4E61_89B3_CD2C338853EB
#define H20F84EDA_8671_4E61_89B3_CD2C338853EB

#include "mcl/keyword.h"
#include "mcl/assert.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_t MclThread;
typedef pthread_attr_t MclThreadAttr;

MCL_INLINE MclStatus MclThread_Create(MclThread *self, const MclThreadAttr *attr, void *(*func)(void*), void *args) {
    int ret = pthread_create(self, attr, func, args);
    if (ret) {
        MCL_LOG_ERR("pthread_create fail %d!", ret);
    }
	return ret ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Detach(MclThread self) {
	return pthread_detach(self) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclThread_Join(MclThread self, void **ext) {
    int ret = pthread_join(self, ext);
    if (ret) {
        MCL_LOG_ERR("pthread_join fail %d!", ret);
    }
    return ret ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE void MclThread_Exit(MclThread *self) {
	pthread_exit(self);
}

MCL_INLINE void MclThread_Yield() {
    sched_yield();
}

#ifndef MCL_OS_MAC
extern int pthread_setname_np(pthread_t, const char*);
extern int pthread_getname_np(pthread_t, char*, size_t);
#endif

MCL_INLINE MclStatus MclThread_SetName(MclThread self, const char *name) {
#ifdef MCL_OS_MAC
	return pthread_setname_np(name) ? MCL_FAILURE : MCL_SUCCESS;
#else
    return pthread_setname_np(self, name) ? MCL_FAILURE : MCL_SUCCESS;
#endif
}

MCL_INLINE MclStatus MclThread_GetName(MclThread self, char *name, size_t len) {
    return pthread_getname_np(self, name, len) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclThread MclThread_GetId() {
    return pthread_self();
}

MCL_STDC_END

#endif
