#ifndef HF595E244_DC76_4DAB_A3AA_4D9FB62983E8
#define HF595E244_DC76_4DAB_A3AA_4D9FB62983E8

#include "mcl/stdc.h"
#include "mcl/fwd_decl.h"
#include "mcl/container_of.h"
#include "mcl/list.h"

#define ROLE(role)	__##role

#define DEFINE_ROLE(role)  			\
MCL_FWD_DECL(role);					\
struct role

#define IMPL_ROLE(role)				\
role ROLE(role)

#define USE_ROLE(role)				\
role* ROLE(role)

#define USE_LIST_ROLE(role)			\
MCL_LIST_HEAD(role) ROLE(role)

#define ROLE_CAST_TO(role, role_ptr, obj, obj_ptr)\
obj* obj_ptr = container_of(role_ptr, obj, ROLE(role))

#define	ROLE_INIT(obj_ptr, role, tbl)\
obj_ptr->ROLE(role) = tbl

#define SELF_ROLE_INIT(role, tbl)	\
ROLE_INIT(self, role, tbl)

#define ROLE_INIT_BY_TBL(obj_ptr, role, ...)	\
do {											\
	role __tbl = __VA_ARGS__;					\
	obj_ptr->ROLE(role) = __tbl;				\
} while(0)

#define SELF_ROLE_INIT_BY_TBL(role, ...)		\
ROLE_INIT_BY_TBL(self, role, ##__VA_ARGS__)

#define ROLE_OF(obj_ptr, role) obj_ptr->ROLE(role)
#define SELF_ROLE(role) ROLE_OF(self, role)
#define SELF_ROLE_P(role) &ROLE_OF(self, role)
#define SELF_INVOKE(role, func, ...) SELF_ROLE(role)->func(SELF_ROLE(role), ##__VA_ARGS__)

#endif
