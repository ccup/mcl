#ifndef H23480AF3_5889_4AEF_BE33_684D1D8313C6
#define H23480AF3_5889_4AEF_BE33_684D1D8313C6

#include "interface/color.h"

MCL_STDC_BEGIN

typedef struct ColorBlue {
	IMPL_INTF(Color);
	int color_id;
}ColorBlue;

void color_blue_init(ColorBlue *);

MCL_STDC_END

#endif
