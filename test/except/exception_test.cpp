#include <cctest/cctest.h>
#include "mcl/except/exception.h"

namespace {
    MclStatus func_return_status(MclStatus status) {
        return status;
    }

    uint32_t demo() {
        MCL_TRY {
            MCL_THROW_IF((1 > 2), 0x1234);
            MCL_THROW_IF_FAIL(func_return_status(MCL_SUCCESS));
            MCL_THROW_IF_FAIL_WITH_PARA(func_return_status((MclStatus)MCL_FAILURE), 1, 2);
            return MCL_SUCCESS;
        }
        MCL_CATCH {
            return __MCL_EXCEPTION_PARA1 + __MCL_EXCEPTION_PARA2;
        }
    }
}

FIXTURE(ExceptionTest) {
    TEST("throw test") {
        MCL_TRY {
            MCL_THROW(12345);
        } MCL_CATCH {
            ASSERT_EQ(__MCL_EXCEPTION_CODE, 12345);
        };
    }
    TEST("throw on condition") {
        MCL_TRY {
            MCL_THROW_IF(false, 12345);
        } MCL_CATCH {
            ASSERT_EQ(__MCL_EXCEPTION_CODE, MCL_NONE_EXCEPTION_CODE);
        };
    }
    TEST("throw if fail") {
        MCL_TRY {
            MCL_THROW_IF_FAIL(func_return_status((MclStatus)0x1234));
        } MCL_CATCH {
            ASSERT_EQ(__MCL_EXCEPTION_CODE, 0x1234);
        };
    }
    TEST("should not throw if success") {
        MCL_TRY {
            MCL_THROW_IF_FAIL(func_return_status(MCL_SUCCESS));
        } MCL_CATCH {
            ASSERT_EQ(__MCL_EXCEPTION_CODE, MCL_SUCCESS);
        };
    }
    TEST("should return right code in demo function") {
        ASSERT_EQ(3, demo());
    }
};
