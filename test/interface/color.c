#include "interface/color.h"
#include "mcl/log.h"

MCL_PRIVATE FillInfo color_ops_render(const Color *color, size_t area) {
	MCL_ERR("Invoke pure abstract function of Color");
	FillInfo info = {NONE_COLOR, 0};
	return info;
}

void color_init(Color *self) {
	DEF_INTF_OPS(Color) {
		.fill = color_ops_render,
	};
	INIT_SELF_INTF_OPS(Color);
}
