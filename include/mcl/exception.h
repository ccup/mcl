#ifndef HDD1B8843_1542_4D8A_ACBC_4D9719CA9B96
#define HDD1B8843_1542_4D8A_ACBC_4D9719CA9B96

#include "mcl/status.h"
#include "mcl/log/log.h"

#define __MCL_EXCEPTION_CODE   (__errorcode)
#define __MCL_EXCEPTION_LINE   (__errorline)
#define __MCL_EXCEPTION_PARA1  (__logpara1)
#define __MCL_EXCEPTION_PARA2  (__logpara2)

#define MCL_NONE_EXCEPTION_CODE 0x7FFFFFCC
#define MCL_NONE_EXCEPTION_LINE 0xFFFFFFFF
#define MCL_NONE_EXCEPTION_PARA 0xFFFFFFFF

#define __MCL_EXCEPT_PROC()                  	\
    {                               			\
        __errorline = __LINE__;     			\
        MCL_LOG_ERR("exception : code = %u, para1 = %u. para2 = %u", __errorcode, __logpara1, __logpara2);\
        goto __catch_position;      			\
    }

#define __MCL_RETURN_IF(expr, errcode)        	\
    {                               			\
        if (expt) {                 			\
            return (errcode);       			\
        }                           			\
    }

#define MCL_TRY                             	\
    MclStatus __errorcode = MCL_NONE_EXCEPTION_CODE;\
    uint32_t  __errorline = MCL_NONE_EXCEPTION_LINE;\
    uint32_t __logpara1 = MCL_NONE_EXCEPTION_PARA;	\
    uint32_t __logpara2 = MCL_NONE_EXCEPTION_PARA; 	\
    (void)__logpara1;                       	\
    (void)__logpara2;                       	\
    (void)__errorline;                      	\
    (void)__errorcode;

#define __MCL_SET_EXCEPT_PARA(para1, para2)     \
    {                                       	\
        __logpara1 = (uint32_t)(para1);			\
        __logpara2 = (uint32_t)(para2);			\
    }

#define MCL_THROW(errcode)                      \
    {                                       	\
        __errorcode = (errcode);            	\
        __MCL_EXCEPT_PROC()                     \
    }

#define MCL_THROW_WITH_PARA(errcode, para1, para2)\
    {                                           \
        __MCL_SET_EXCEPT_PARA(para1, para2);    \
        MCL_THROW(errcode)                      \
    }

#define MCL_THROW_WITH_PROC(errcode, errorfunc) \
    {                                           \
        errorfunc;                              \
        MCL_THROW(errcode)                      \
    }

#define MCL_THROW_IF(expr, errcode)             \
    {                                           \
        if (expr) {                             \
            MCL_THROW(errcode)                  \
        }                                       \
    }

#define MCL_THROW_IF_WITH_PROC(expr, errcode, errorfunc) \
    {                                           \
        if (expr) {                             \
            errorfunc;                          \
            MCL_THROW(errcode)                  \
        }                                       \
    }

#define MCL_THROW_IF_WITH_PARA(expr, errcode, para1, para2)\
    {                                           \
        if (expr) {                             \
            __MCL_SET_EXCEPT_PARA(para1, para2);\
            MCL_THROW(errcode)                  \
        }                                       \
    }

#define MCL_THROW_IF_FAIL(func)                 \
    {   										\
		__errorcode = (MclStatus)(func);		\
        if (MCL_FAILED(__errorcode)) {			\
            __MCL_EXCEPT_PROC()                 \
        }                                       \
    }

#define MCL_THROW_IF_FAIL_WITH_ERR(func, errcode) \
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (MCL_FAILED(__errorcode)) {			\
            MCL_THROW(errcode)                  \
        }                                       \
    }

#define MCL_THROW_IF_FAIL_WITH_PROC(func, errorfunc)\
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (MCL_FAILED(__errorcode)) {			\
            errorfunc;                          \
            __MCL_EXCEPT_PROC()                 \
        }                                       \
    }

#define MCL_THROW_IF_FAIL_WITH_PARA(func, para1, para2)\
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (MCL_FAILED(__errorcode)) {			\
            __MCL_SET_EXCEPT_PARA(para1, para2);\
            __MCL_EXCEPT_PROC()                 \
        }                                       \
    }

#define MCL_CATCH                               \
    __catch_position:

#endif
