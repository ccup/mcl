#ifndef H5712E60C_DEF5_4A3B_93C1_C074CD80C63C
#define H5712E60C_DEF5_4A3B_93C1_C074CD80C63C

#include "mcl/status.h"
#include "mcl/log.h"
#include <stddef.h>
#include <stdbool.h>

#define __MCL_FAILED_BOOL_ACTION      return false;
#define __MCL_FAILED_STATUS_ACTION    return MCL_FAILURE;
#define __MCL_STATUS_ACTION           return __status;
#define __MCL_VOID_ACTION             return;
#define __MCL_NIL_ACTION              return 0;
#define __MCL_PEEK_ACTION
#define __MCL_RESULT_ACTION(status)   return status;

//////////////////////////////////////////////////////////////////////////
#define __MCL_WARNING_NOT_TRUE(exp)         \
  MCL_LOG_WARN("assertion failed: "#exp)


#define __MCL_EXPECT_TRUE(exp, action)      \
do {                                        \
    if (!(exp))                             \
    {                                       \
        __MCL_WARNING_NOT_TRUE(exp);        \
        action;                             \
    }                                       \
} while (0)

#define MCL_EXPECT_TRUE_R(exp, status)  \
    __MCL_EXPECT_TRUE(exp, __MCL_RESULT_ACTION(status))

#define MCL_EXPECT_TRUE(exp)            \
  __MCL_EXPECT_TRUE(exp, __MCL_FAILED_STATUS_ACTION)

#define MCL_EXPECT_FALSE(exp)           \
    __MCL_EXPECT_TRUE(!(exp), __MCL_FAILED_STATUS_ACTION)

#define MCL_EXPECT_FALSE_R(exp, status) \
    __MCL_EXPECT_TRUE(!(exp), __MCL_RESULT_ACTION(status))

#define MCL_EXPECT_TRUE_NIL(exp)        \
  __MCL_EXPECT_TRUE(exp, __MCL_NIL_ACTION)

#define MCL_EXPECT_TRUE_VOID(exp)       \
  __MCL_EXPECT_TRUE(exp, __MCL_VOID_ACTION)

#define MCL_EXPECT_TRUE_BOOL(exp)       \
  __MCL_EXPECT_TRUE(exp, __MCL_FAILED_BOOL_ACTION)

///////////////////////////////////////////////////////////////
#define  __MCL_NOT_TRUE(exp)                \
    MCL_LOG_ERR("assertion failed: "#exp)

#define __MCL_ASSERT_TRUE(exp, action)      \
do {                                        \
    if (!(exp))                             \
    {                                       \
        __MCL_NOT_TRUE(exp);                \
        action;                             \
    }                                       \
} while (0)

#define MCL_ASSERT_TRUE_R(exp, status)  \
    __MCL_ASSERT_TRUE(exp, __MCL_RESULT_ACTION(status))

#define MCL_ASSERT_TRUE(exp)            \
  __MCL_ASSERT_TRUE(exp, __MCL_FAILED_STATUS_ACTION)

#define MCL_ASSERT_FALSE(exp)           \
    __MCL_ASSERT_TRUE(!(exp), __MCL_FAILED_STATUS_ACTION)

#define MCL_ASSERT_FALSE_R(exp, status) \
    __MCL_ASSERT_TRUE(!(exp), __MCL_RESULT_ACTION(status))

#define MCL_ASSERT_TRUE_NIL(exp)        \
  __MCL_ASSERT_TRUE(exp, __MCL_NIL_ACTION)

#define MCL_PEEK_TRUE(exp)              \
  __MCL_ASSERT_TRUE(exp, __MCL_PEEK_ACTION)

#define MCL_ASSERT_TRUE_VOID(exp)       \
  __MCL_ASSERT_TRUE(exp, __MCL_VOID_ACTION)

#define MCL_ASSERT_TRUE_BOOL(exp)       \
  __MCL_ASSERT_TRUE(exp, __MCL_FAILED_BOOL_ACTION)

//////////////////////////////////////////////////////////////////////////
#define __MCL_ASSERT_TRUE_POST(exp, action, returnAction) \
do {                                        \
    if (!(exp))                             \
    {                                       \
        __MCL_NOT_TRUE(exp);                \
        action;                             \
        returnAction;                       \
    }                                       \
} while (0)

#define MCL_ASSERT_TRUE_POST(exp, action)   \
  __MCL_ASSERT_TRUE_POST(exp, action, __MCL_FAILED_STATUS_ACTION)


//////////////////////////////////////////////////////////////////////////
#define __MCL_WARNING_CALL(call, status)        \
    MCL_WARN(#call" = [%X]", status)

#define __MCL_EXPECT_SUCC_CALL(call, action)\
do {                                            \
    MclStatus __status = call;                  \
    if (MCL_FAILED(__status))                   \
    {                                           \
        __MCL_WARNING_CALL(call, __status);     \
        action;                                 \
    }                                           \
} while (0)

#define MCL_EXPECT_SUCC_CALL_R(call, status)    \
   __MCL_EXPECT_SUCC_CALL(call, __MCL_RESULT_ACTION(status))

#define MCL_EXPECT_SUCC_CALL(call)              \
   __MCL_EXPECT_SUCC_CALL(call, __MCL_STATUS_ACTION)

#define MCL_EXPECT_SUCC_CALL_BOOL(call)         \
    __MCL_EXPECT_SUCC_CALL(call, __MCL_FAILED_BOOL_ACTION)

#define MCL_EXPECT_SUCC_CALL_VOID(call)         \
   __MCL_EXPECT_SUCC_CALL(call, __MCL_VOID_ACTION)

#define MCL_EXPECT_SUCC_CALL_NIL(call)          \
   __MCL_EXPECT_SUCC_CALL(call, __MCL_NIL_ACTION)

/////////////////////////////////////////////////////////////////////
#define __MCL_FAIL_CALL(call, status)           \
  MCL_LOG_ERR(#call" = [%X]", status)

#define __MCL_ASSERT_SUCC_CALL(call, action)    \
do {                                            \
    MclStatus __status = call;                  \
    if (MCL_FAILED(__status))                   \
    {                                           \
        __MCL_FAIL_CALL(call, __status);        \
        action;                                 \
    }                                           \
} while (0)

#define MCL_ASSERT_SUCC_CALL_R(call, status)    \
    __MCL_ASSERT_SUCC_CALL(call, __MCL_RESULT_ACTION(status))

#define MCL_ASSERT_SUCC_CALL(call)          \
   __MCL_ASSERT_SUCC_CALL(call, __MCL_STATUS_ACTION)

#define MCL_ASSERT_SUCC_CALL_BOOL(call)     \
    __MCL_ASSERT_SUCC_CALL(call, __MCL_FAILED_BOOL_ACTION)

#define MCL_ASSERT_SUCC_CALL_VOID(call)     \
   __MCL_ASSERT_SUCC_CALL(call, __MCL_VOID_ACTION)

#define MCL_ASSERT_SUCC_CALL_NIL(call)      \
   __MCL_ASSERT_SUCC_CALL(call, __MCL_NIL_ACTION)

/*no return even though call failed.*/
#define MCL_PEEK_SUCC_CALL(call)            \
   __MCL_ASSERT_SUCC_CALL(call, __MCL_PEEK_ACTION)

//////////////////////////////////////////////////////////////////////////

#define __MCL_ASSERT_SUCC_CALL_FINALLY(call, action, returnAction)  \
do {                                                \
    MclStatus __status = call;                      \
    if (MCL_FAILED(__status))                       \
    {                                               \
        __MCL_FAIL_CALL(call, __status);            \
        action;                                     \
        returnAction;                               \
    }                                               \
} while (0)

#define MCL_ASSERT_SUCC_CALL_FINALLY(exp, action)   \
  __MCL_ASSERT_SUCC_CALL_FINALLY(exp, action, __MCL_FAILED_STATUS_ACTION)

//////////////////////////////////////////////////////////////////////////
#define __MCL_ASSERT_NO_REAL_FAIL(call, action)      \
do {                                                 \
    MclStatus __status = call;                       \
    if(__status == MCL_NOTHING_CHANGED)              \
    {                                                \
        return MCL_SUCCESS;                          \
    }                                                \
    else if (MCL_FAILED(__status))                   \
    {                                                \
        __MCL_FAIL_CALL(call, __status);             \
        action;                                      \
    }                                                \
} while (0)

#define MCL_ASSERT_NO_REAL_FAIL(call)                \
    __MCL_ASSERT_NO_REAL_FAIL(call, __MCL_STATUS_ACTION)

//////////////////////////////////////////////////////////////////////////
#define __MCL_NIL_PTR(ptr)                               \
    MCL_LOG_ERR("assertion failed: unexpected null ptr: "#ptr)

#define __MCL_ASSERT_VALID_PTR(ptr, action)          \
do {                                                 \
    if ((ptr) == NULL)                         		 \
    {                                                \
        __MCL_NIL_PTR(ptr);                          \
        action;                                      \
    }                                                \
} while (0)

#define MCL_ASSERT_VALID_PTR_R(ptr, status)     \
    __MCL_ASSERT_VALID_PTR(ptr, __MCL_RESULT_ACTION(status))

#define MCL_ASSERT_VALID_PTR(ptr)               \
  __MCL_ASSERT_VALID_PTR(ptr, __MCL_FAILED_STATUS_ACTION)

#define MCL_ASSERT_VALID_PTR_BOOL(ptr)          \
   __MCL_ASSERT_VALID_PTR(ptr, __MCL_FAILED_BOOL_ACTION)

#define MCL_ASSERT_VALID_PTR_VOID(ptr)          \
  __MCL_ASSERT_VALID_PTR(ptr, __MCL_VOID_ACTION)

#define MCL_PEEK_VALID_PTR(ptr)                 \
  __MCL_ASSERT_VALID_PTR(ptr, __MCL_PEEK_ACTION)

#define MCL_ASSERT_VALID_PTR_NIL(ptr)           \
  __MCL_ASSERT_VALID_PTR(ptr, __MCL_NIL_ACTION)

#define __MCL_ASSERT_VALID_PTR_FINALLY(ptr, action, returnAction)  \
    if ((ptr) == __null_ptr)                                       \
    {                                                              \
        __MCL_NIL_PTR(ptr);                                        \
        action;                                                    \
        returnAction;                                              \
    }

#define MCL_PEEK_VALID_PTR_FINALLY(ptr, action)               \
    __MCL_ASSERT_VALID_PTR_FINALLY(ptr, action, __MCL_PEEK_ACTION)

#define MCL_ASSERT_VALID_PTR_NIL_FINALLY(ptr, action)         \
    __MCL_ASSERT_VALID_PTR_FINALLY(ptr, action, __MCL_NIL_ACTION)


#endif
