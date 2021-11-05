#ifndef H681BE94E_59CF_492D_A77C_208C71BC5F07
#define H681BE94E_59CF_492D_A77C_208C71BC5F07

#include "interface/shape.h"
#include "mcl/fwd_decl.h"
#include <stddef.h>

MCL_STDC_BEGIN

MCL_FWD_DECL(Color);

typedef struct ShapeCircle {
	MCL_INTF_IMPL(Shape);
	MCL_DEPS_INTF(Color);
	size_t radius;
}ShapeCircle;

void shape_circle_init(ShapeCircle*, Color*, size_t radius);

MCL_STDC_END

#endif
