#include "interface/shape.h"
#include "mcl/log/log.h"

MCL_PRIVATE ShapeInfo shape_ops_draw(const Shape *shape) {
	MCL_LOG_ERR("Invoke pure abstract function of Shape");
	ShapeInfo info = {NONE_SHAPE, NONE_COLOR, 0};
	return info;
}


void shape_init(Shape *self) {
	MCL_INTF_DEF_OPS(Shape) {
		.draw = shape_ops_draw,
	};
	MCL_INTF_INIT_SELF_OPS(Shape);
}
