#ifndef H3B07E0A9_E2B1_4D74_BB3D_2A4A4F6C98BF
#define H3B07E0A9_E2B1_4D74_BB3D_2A4A4F6C98BF

#include "interface/shape_type.h"
#include "interface/color_type.h"
#include <stddef.h>

MCL_STDC_BEGIN

typedef struct ShapeInfo {
	ShapeType shape;
	ColorType color;
	size_t area;
}ShapeInfo;

MCL_STDC_END

#endif
