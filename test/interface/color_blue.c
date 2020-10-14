#include "interface/color_blue.h"
#include "mcl/log.h"

MCL_PRIVATE FillInfo color_blue_ops_render(const Color *color, size_t area) {
	INTF_CAST_TO(Color, color, ColorBlue, self);
	MCL_INFO("blue color id = %d draw area %d", self->color_id, area);
	FillInfo info = {BLUE, area};
	return info;
}

void color_blue_init(ColorBlue *self) {
	DEF_INTF_OPS(Color) {
		.fill = color_blue_ops_render,
	};
	color_init(&self->INTF(Color));
	IMPL_SELF_INTF_OPS(Color);
	self->color_id = 5;
}
