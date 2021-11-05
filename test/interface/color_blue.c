#include "interface/color_blue.h"
#include "mcl/log/log.h"

MCL_PRIVATE FillInfo color_blue_ops_render(const Color *color, size_t area) {
	MCL_INTF_CAST_TO(Color, color, ColorBlue, self);
	MCL_LOG_INFO("blue color id = %d draw area %lu", self->color_id, area);
	FillInfo info = {BLUE, area};
	return info;
}

void color_blue_init(ColorBlue *self) {
	MCL_INTF_DEF_OPS(Color) {
		.fill = color_blue_ops_render,
	};
	color_init(&self->MCL_INTF(Color));
	MCL_INTF_IMPL_SELF_OPS(Color);
	self->color_id = 5;
}
