#ifndef H5D31D8DF_B49E_41F5_A9FC_D9FD53F01E1E
#define H5D31D8DF_B49E_41F5_A9FC_D9FD53F01E1E

#include "mcl/keyword.h"
#include "mcl/container_of.h"

MCL_STDC_BEGIN

#define INTF_OPS_TYPE(TYPE)   TYPE##Ops
#define INTF_OPS_INST(TYPE)   TYPE##_ops

#define DEF_INTERFACE(TYPE)								\
typedef struct INTF_OPS_TYPE(TYPE) INTF_OPS_TYPE(TYPE);	\
typedef struct TYPE TYPE;								\
struct TYPE {											\
	INTF_OPS_TYPE(TYPE) *ops;							\
};														\
struct INTF_OPS_TYPE(TYPE)

#define DEF_INTF_OPS(TYPE)								\
MCL_PRIVATE INTF_OPS_TYPE(TYPE) INTF_OPS_INST(TYPE) =

#define INTF(TYPE)	__##TYPE

#define IMPL_INTF(TYPE)	 TYPE INTF(TYPE)
#define DEPS_INTF(TYPE)  TYPE* INTF(TYPE)

#define INTF_CAST_TO(INTF_TYPE, INTF_PTR, TYPE, TYPE_PTR)\
TYPE* TYPE_PTR = container_of(INTF_PTR, TYPE, INTF(INTF_TYPE))

#define INTF_OF(TYPE_PTR, INTF_TYPE) (TYPE_PTR->INTF(INTF_TYPE))
#define SELF_INTF(INTF_TYPE) INTF_OF(self, INTF_TYPE)
#define SELF_INTF_P(INTF_TYPE) (&INTF_OF(self, INTF_TYPE))

#define INIT_INTF_OPS(INTF_PTR, INTF_TYPE) INTF_PTR->ops = &INTF_OPS_INST(INTF_TYPE)
#define IMPL_INTF_OPS(TYPE_PTR, INTF_TYPE) INIT_INTF_OPS((&INTF_OF(TYPE_PTR, INTF_TYPE)), INTF_TYPE)

#define INIT_SELF_INTF_OPS(INTF_TYPE) INIT_INTF_OPS(self, INTF_TYPE)
#define IMPL_SELF_INTF_OPS(INTF_TYPE) INIT_INTF_OPS(SELF_INTF_P(INTF_TYPE), INTF_TYPE)

#define DEPS_OF(TYPE_PTR, DEPS_TYPE) (TYPE_PTR->INTF(DEPS_TYPE))
#define SELF_DEPS(DEPS_TYPE) DEPS_OF(self, DEPS_TYPE)

MCL_STDC_END

#endif