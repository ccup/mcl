#ifndef HCE6F2EA4_7984_4AE2_B6CF_4EF05DFCBE0A
#define HCE6F2EA4_7984_4AE2_B6CF_4EF05DFCBE0A

#include "interface/shape_info.h"
#include "mcl/interface.h"
#include "mcl/keyword.h"

MCL_STDC_BEGIN

MCL_INTF_DEF(Shape) {
	ShapeInfo (*draw)(const Shape*);
};

void shape_init(Shape*);

MCL_INLINE ShapeInfo shape_draw(Shape *shape) {
	return shape->ops->draw(shape);
}

MCL_STDC_END

#endif
