#include "interface/color_red.h"
#include "mcl/log.h"

MCL_PRIVATE FillInfo color_red_ops_render(const Color *color, size_t area) {
	MCL_INTF_CAST_TO(Color, color, ColorRed, self);
	MCL_INFO("%s color draw %d area", self->color_name, area);
	FillInfo info = {RED, area};
	return info;
}

void color_red_init(ColorRed *self) {
	MCL_INTF_DEF_OPS(Color) {
		.fill = color_red_ops_render,
	};
	color_init(&self->MCL_INTF(Color));
	MCL_INTF_IMPL_SELF_OPS(Color);
	self->color_name = "red";
}
