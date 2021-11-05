#include "interface/shape_rectangle.h"
#include "interface/color.h"
#include "interface/fill_info.h"
#include "mcl/log/log.h"

MCL_PRIVATE ShapeInfo shape_rectangle_ops_draw(const Shape *shape) {
	MCL_INTF_CAST_TO(Shape, shape, ShapeRectangle, self);
	MCL_LOG_INFO("rectangle draw height %lu, width = %lu", self->height, self->width);
	size_t area = self->width * self->height;
	FillInfo info = color_fill(MCL_INTF_SELF_DEPS(Color), area);
	ShapeInfo result = {.shape = RECTANGLE, .color = info.color, .area = info.area};
	return result;
}

void shape_rectangle_init(ShapeRectangle* self, Color* color, size_t width, size_t height) {
	MCL_INTF_DEF_OPS(Shape) {
		.draw = shape_rectangle_ops_draw,
	};
	shape_init(&self->MCL_INTF(Shape));
	MCL_INTF_IMPL_SELF_OPS(Shape);
	MCL_INTF_SELF_DEPS(Color) = color;
	self->height = height;
	self->width = width;
}
