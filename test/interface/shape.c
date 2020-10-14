#include "interface/shape.h"
#include "mcl/log.h"

MCL_PRIVATE ShapeInfo shape_ops_draw(const Shape *shape) {
	MCL_ERR("Invoke pure abstract function of Shape");
	ShapeInfo info = {NONE_SHAPE, NONE_COLOR, 0};
	return info;
}


void shape_init(Shape *self) {
	DEF_INTF_OPS(Shape) {
		.draw = shape_ops_draw,
	};
	INIT_SELF_INTF_OPS(Shape);
}
