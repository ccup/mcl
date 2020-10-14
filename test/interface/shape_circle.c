#include "interface/shape_circle.h"
#include "interface/color.h"
#include "interface/fill_info.h"
#include "mcl/log.h"

const static size_t PI = 3;

MCL_PRIVATE ShapeInfo shape_circle_ops_draw(const Shape *shape) {
	INTF_CAST_TO(Shape, shape, ShapeCircle, self);
	MCL_INFO("circle draw radius = %d", self->radius);
	size_t area = PI * self->radius * self->radius;
	FillInfo info = color_fill(SELF_DEPS(Color), area);
	ShapeInfo result = {.shape = CIRCLE, .color = info.color, .area = info.area};
	return result;
}

void shape_circle_init(ShapeCircle* self, Color* color, size_t radius) {
	DEF_INTF_OPS(Shape) {
		.draw = shape_circle_ops_draw,
	};
	shape_init(&self->INTF(Shape));
	IMPL_SELF_INTF_OPS(Shape);
	SELF_DEPS(Color) = color;
	self->radius = radius;
}
