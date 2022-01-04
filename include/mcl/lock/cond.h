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

MCL_INLINE MclStatus MclCond_Init(MclCond *cond, const MclCondAttr *attr) {
	return pthread_cond_init(cond, attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Destroy(MclCond *cond) {
	return pthread_cond_destroy(cond) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Signal(MclCond *cond) {
	return pthread_cond_signal(cond) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Broadcast(MclCond *cond) {
	return pthread_cond_broadcast(cond) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_Wait(MclCond *cond, MclMutex *mutex) {
	return pthread_cond_wait(cond, mutex) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclCond_TimedWait(MclCond *cond, MclMutex *mutex, MclCondTimeSpec *abstime) {
	return pthread_cond_timedwait(cond, mutex, abstime) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_STDC_END

#endif
