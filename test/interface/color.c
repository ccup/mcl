#include "interface/color.h"
#include "mcl/log/log.h"

MCL_PRIVATE FillInfo color_ops_render(const Color *color, size_t area) {
	MCL_LOG_ERR("Invoke pure abstract function of Color");
	FillInfo info = {NONE_COLOR, 0};
	return info;
}

void color_init(Color *self) {
	MCL_INTF_DEF_OPS(Color) {
		.fill = color_ops_render,
	};
	MCL_INTF_INIT_SELF_OPS(Color);
}
