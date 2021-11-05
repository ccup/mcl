#ifndef HF595E244_DC76_4DAB_A3AA_4D9FB62983E8
#define HF595E244_DC76_4DAB_A3AA_4D9FB62983E8

#include "mcl/stdc.h"
#include "mcl/typedef.h"
#include "mcl/container_of.h"
#include "mcl/list.h"

#define MCL_ROLE(role)	__##role

#define MCL_ROLE_DEF(role)  			\
MCL_TYPE_FWD(role);						\
struct role

#define MCL_ROLE_IMPL(role)				\
role MCL_ROLE(role)

#define MCL_ROLE_USE(role)				\
role* MCL_ROLE(role)

#define MCL_ROLE_LIST_USE(role)			\
MCL_LIST_HEAD(role) MCL_ROLE(role)

#define MCL_ROLE_CAST_TO(role, role_ptr, obj, obj_ptr)\
obj* obj_ptr = MCL_CONTAINER_OF(role_ptr, obj, MCL_ROLE(role))

#define	MCL_ROLE_INIT(obj_ptr, role, tbl)\
obj_ptr->MCL_ROLE(role) = tbl

#define MCL_ROLE_SELF_INIT(role, tbl)	\
MCL_ROLE_INIT(self, role, tbl)

#define MCL_ROLE_INIT_BY_TBL(obj_ptr, role, ...)\
do {											\
	role __tbl = __VA_ARGS__;					\
	obj_ptr->MCL_ROLE(role) = __tbl;			\
} while(0)

#define MCL_ROLE_INIT_SELF_BY_TBL(role, ...)	\
MCL_ROLE_INIT_BY_TBL(self, role, ##__VA_ARGS__)

#define MCL_ROLE_OF(obj_ptr, role) obj_ptr->MCL_ROLE(role)
#define MCL_ROLE_SELF(role)  MCL_ROLE_OF(self, role)
#define MCL_ROLE_SELF_P(role) &MCL_ROLE_OF(self, role)
#define MCL_ROLE_SELF_INVOKE(role, func, ...) MCL_ROLE_SELF(role)->func(MCL_ROLE_SELF(role), ##__VA_ARGS__)

#endif
