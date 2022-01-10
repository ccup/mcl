#ifndef H16533543_F5E3_4361_BF24_AEB9E4E92F5B
#define H16533543_F5E3_4361_BF24_AEB9E4E92F5B

#include "mcl/lock/mutex.h"

MCL_STDC_BEGIN

typedef pthread_cond_t MclCond;
typedef pthread_condattr_t MclCondAttr;
typedef struct timespec MclCondTimeSpec;

#define MCL_COND()   PTHREAD_COND_INITIALIZER

MCL_INLINE MclStatus MclCond_InitAttr(MclCondAttr *attr) {
	return pthread_condattr_init(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_DestroyAttr(MclCondAttr *attr) {
	return pthread_condattr_destroy(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_SetAttrPShared(MclCondAttr *attr, int pshared) {
	return pthread_condattr_setpshared(attr, pshared) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Init(MclCond *self, const MclCondAttr *attr) {
	return pthread_cond_init(self, attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Destroy(MclCond *self) {
	return pthread_cond_destroy(self) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Signal(MclCond *self) {
	return pthread_cond_signal(self) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Broadcast(MclCond *self) {
	return pthread_cond_broadcast(self) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Wait(MclCond *self, MclMutex *mutex) {
	return pthread_cond_wait(self, mutex) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_TimedWait(MclCond *self, MclMutex *mutex, MclCondTimeSpec *abstime) {
	return pthread_cond_timedwait(self, mutex, abstime) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_STDC_END

#endif
