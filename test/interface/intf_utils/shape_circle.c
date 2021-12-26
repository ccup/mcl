#include "interface/intf_utils/shape_circle.h"
#include "interface/intf_utils/color.h"
#include "interface/intf_utils/fill_info.h"
#include "mcl/log/log.h"

const static size_t PI = 3;

MCL_PRIVATE ShapeInfo shape_circle_ops_draw(const Shape *shape) {
	MCL_INTF_CAST_TO(Shape, shape, ShapeCircle, self);
	MCL_LOG_INFO("circle draw radius = %lu", self->radius);
	size_t area = PI * self->radius * self->radius;
	FillInfo info = color_fill(MCL_INTF_SELF_DEPS(Color), area);
	ShapeInfo result = {.shape = CIRCLE, .color = info.color, .area = info.area};
	return result;
}

void shape_circle_init(ShapeCircle* self, Color* color, size_t radius) {
	MCL_INTF_DEF_OPS(Shape) {
		.draw = shape_circle_ops_draw,
	};
	shape_init(&self->MCL_INTF(Shape));
	MCL_INTF_IMPL_SELF_OPS(Shape);
	MCL_INTF_SELF_DEPS(Color) = color;
	self->radius = radius;
}
