#ifndef HDD1B8843_1542_4D8A_ACBC_4D9719CA9B96
#define HDD1B8843_1542_4D8A_ACBC_4D9719CA9B96

#include "mcl/status.h"
#include "mcl/log.h"

#define __EXCEPTION_CODE   (__errorcode)
#define __EXCEPTION_LINE   (__errorline)
#define __EXCEPTION_PARA1  (__logpara1)
#define __EXCEPTION_PARA2  (__logpara2)

#define NONE_EXCEPTION_CODE 0x7FFFFFCC
#define NONE_EXCEPTION_LINE 0xFFFFFFFF
#define NONE_EXCEPTION_PARA 0xFFFFFFFF

#define __EXCEPT_PROC()                  		\
    {                               			\
        __errorline = __LINE__;     			\
        MCL_ERR("exception : code = %u, para1 = %u. para2 = %u", __errorcode, __logpara1, __logpara2);\
        goto __catch_position;      			\
    }

#define __RETURN_IF(expr, errcode)        		\
    {                               			\
        if (expt) {                 			\
            return (errcode);       			\
        }                           			\
    }

#define __TRY__                             	\
    MclStatus __errorcode = NONE_EXCEPTION_CODE;\
    uint32_t  __errorline = NONE_EXCEPTION_LINE;\
    uint32_t __logpara1 = NONE_EXCEPTION_PARA;	\
    uint32_t __logpara2 = NONE_EXCEPTION_PARA; 	\
    (void)__logpara1;                       	\
    (void)__logpara2;                       	\
    (void)__errorline;                      	\
    (void)__errorcode;

#define __SET_EXCEPT_PARA(para1, para2)       	\
    {                                       	\
        __logpara1 = (uint32_t)(para1);			\
        __logpara2 = (uint32_t)(para2);			\
    }

#define __THROW__(errcode)                      \
    {                                       	\
        __errorcode = (errcode);            	\
        __EXCEPT_PROC()                       	\
    }

#define __THROW_WITH_PARA(errcode, para1, para2)\
    {                                           \
        __SET_EXCEPT_PARA(para1, para2);      	\
        __THROW__(errcode)                      \
    }

#define __THROW_WITH_PROC(errcode, errorfunc)   \
    {                                           \
        errorfunc;                              \
        __THROW__(errcode)                      \
    }

#define __THROW_IF(expr, errcode)               \
    {                                           \
        if (expr) {                             \
            __THROW__(errcode)                  \
        }                                       \
    }

#define __THROW_IF_WITH_PROC(expr, errcode, errorfunc) \
    {                                           \
        if (expr) {                             \
            errorfunc;                          \
            __THROW__(errcode)                  \
        }                                       \
    }

#define __THROW_IF_WITH_PARA(expr, errcode, para1, para2)\
    {                                           \
        if (expr) {                             \
            __SET_EXCEPT_PARA(para1, para2);  	\
            __THROW__(errcode)                  \
        }                                       \
    }

#define __THROW_IF_FAIL(func)                   \
    {   										\
		__errorcode = (MclStatus)(func);		\
        if (__MCL_FAILED(__errorcode)) {		\
            __EXCEPT_PROC()                   	\
        }                                       \
    }

#define __THROW_IF_FAIL_WITH_ERR(func, errcode) \
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (__MCL_FAILED(__errorcode)) {		\
            __THROW__(errcode)                  \
        }                                       \
    }

#define __THROW_IF_FAIL_WITH_PROC(func, errorfunc)\
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (__MCL_FAILED(__errorcode)) {		\
            errorfunc;                          \
            __EXCEPT_PROC()                   	\
        }                                       \
    }

#define __THROW_IF_FAIL_WITH_PARA(func, para1, para2)\
    {                                           \
		__errorcode = (MclStatus)(func);		\
		if (__MCL_FAILED(__errorcode)) {		\
            __SET_EXCEPT_PARA(para1, para2);  	\
            __EXCEPT_PROC()                   	\
        }                                       \
    }

#define __CATCH__                               \
    __catch_position:

#endif
