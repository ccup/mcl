#include <cctest/cctest.h>
#include "mcl/lock/rwlock.h"

FIXTURE(RwlockTest)
{
    TEST("should failed when destroy locked rwlock")
    {
    	MclRwLock rwlock = MCL_RWLOCK();

    	MclRwLock_RdLock(&rwlock);

        ASSERT_TRUE(MCL_FAILED(MclRwLock_Destroy(&rwlock)));
    }

    TEST("should not failed when unlock none locked rwlock")
    {
    	MclRwLock rwlock = MCL_RWLOCK();

    	ASSERT_TRUE(!MCL_FAILED(MclRwLock_UnLock(&rwlock)));

        ASSERT_TRUE(!MCL_FAILED(MclRwLock_Destroy(&rwlock)));
    }

    TEST("should read lock on same rwlock multiple times")
    {
    	MclRwLock rwlock = MCL_RWLOCK();

    	MclRwLock_RdLock(&rwlock);
    	MclRwLock_RdLock(&rwlock);

    	MclRwLock_UnLock(&rwlock);

    	MclRwLock_RdLock(&rwlock);

    	MclRwLock_UnLock(&rwlock);
    	MclRwLock_UnLock(&rwlock);

        ASSERT_TRUE(!MCL_FAILED(MclRwLock_Destroy(&rwlock)));
    }
};
