#ifndef H83606A33_B7EE_4211_BAD9_C147742346D7
#define H83606A33_B7EE_4211_BAD9_C147742346D7

#include "interface/fill_info.h"
#include "mcl/interface.h"
#include "mcl/keyword.h"
#include <stddef.h>

MCL_STDC_BEGIN

MCL_INTF_DEF(Color) {
	FillInfo (*fill)(const Color*, size_t area);
};

void color_init(Color *);

MCL_INLINE FillInfo color_fill(Color *color, size_t area)
{
	return color->ops->fill(color, area);
}

MCL_STDC_END

#endif
