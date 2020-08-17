#include <cctest/cctest.h>
#include "mcl/exception.h"

namespace {
    MclStatus func_return_status(MclStatus status) {
        return status;
    }

    uint32_t demo() {
        __TRY
        {
            __THROW_IF((1 > 2), 0x1234);
            __THROW_IF_FAIL(func_return_status(MCL_SUCCESS));
            __THROW_IF_FAIL_WITH_PARA(func_return_status(0xFFFFFFFF), 1, 2);
            return MCL_SUCCESS;
        }
        __CATCH
        {
            return __EXCEPTION_PARA1 + __EXCEPTION_PARA2;
        }
    }
}

FIXTURE(ExceptionTest) {
    TEST("throw test") {
        __TRY {
            __THROW(12345);
        } __CATCH {
            ASSERT_EQ(__EXCEPTION_CODE, 12345);
        };
    }
    TEST("throw on condition") {
        __TRY {
            __THROW_IF(false, 12345);
        } __CATCH {
            ASSERT_EQ(__EXCEPTION_CODE, NONE_EXCEPTION_CODE);
        };
    }
    TEST("throw if fail") {
        __TRY {
            __THROW_IF_FAIL(func_return_status(0x1234));
        } __CATCH {
            ASSERT_EQ(__EXCEPTION_CODE, 0x1234);
        };
    }
    TEST("should not throw if success") {
        __TRY {
            __THROW_IF_FAIL(func_return_status(MCL_SUCCESS));
        } __CATCH {
            ASSERT_EQ(__EXCEPTION_CODE, MCL_SUCCESS);
        };
    }
    TEST("should return right code in demo function") {
        ASSERT_EQ(3, demo());
    }
};
