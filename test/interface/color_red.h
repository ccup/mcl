#ifndef HAF8EE6E1_2D2C_453A_B321_A5CEDA824A46
#define HAF8EE6E1_2D2C_453A_B321_A5CEDA824A46

#include "interface/color.h"

MCL_STDC_BEGIN

typedef struct ColorRed {
	IMPL_INTF(Color);
	const char* color_name;
}ColorRed;

void color_red_init(ColorRed *);

MCL_STDC_END

#endif
