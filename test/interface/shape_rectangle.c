#include "interface/shape_rectangle.h"
#include "interface/color.h"
#include "interface/fill_info.h"
#include "mcl/log.h"

MCL_PRIVATE ShapeInfo shape_rectangle_ops_draw(const Shape *shape) {
	INTF_CAST_TO(Shape, shape, ShapeRectangle, self);
	MCL_INFO("rectangle draw height %d, width = %d", self->height, self->width);
	size_t area = self->width * self->height;
	FillInfo info = color_fill(SELF_DEPS(Color), area);
	ShapeInfo result = {.shape = RECTANGLE, .color = info.color, .area = info.area};
	return result;
}

void shape_rectangle_init(ShapeRectangle* self, Color* color, size_t width, size_t height) {
	DEF_INTF_OPS(Shape) {
		.draw = shape_rectangle_ops_draw,
	};
	shape_init(&self->INTF(Shape));
	IMPL_SELF_INTF_OPS(Shape);
	SELF_DEPS(Color) = color;
	self->height = height;
	self->width = width;
}
