#ifndef H681BE94E_59CF_492D_A77C_208C71BC5F06
#define H681BE94E_59CF_492D_A77C_208C71BC5F06

#include "interface/shape.h"
#include "mcl/typedef.h"
#include <stddef.h>

MCL_STDC_BEGIN

MCL_TYPE_FWD(Color);

typedef struct ShapeRectangle {
	MCL_INTF_IMPL(Shape);
	MCL_DEPS_INTF(Color);
	size_t width, height;
}ShapeRectangle;

void shape_rectangle_init(ShapeRectangle*, Color*, size_t width, size_t height);

MCL_STDC_END

#endif
