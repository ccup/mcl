#include <cctest/cctest.h>
#include "interface/color_blue.h"
#include "interface/color_red.h"
#include "interface/shape_rectangle.h"
#include "interface/shape_circle.h"

FIXTURE(BridgeTest) {
	TEST("should fill by blue color") {
		ColorBlue blue;
		color_blue_init(&blue);
		FillInfo info = color_fill(&blue.MCL_INTF(Color), 12);
		ASSERT_EQ(BLUE, info.color);
		ASSERT_EQ(12, info.area);
	}

	TEST("should fill by red color") {
		ColorRed red;
		color_red_init(&red);
		FillInfo info = color_fill(&red.MCL_INTF(Color), 10);
		ASSERT_EQ(RED, info.color);
		ASSERT_EQ(10, info.area);
	}

	TEST("should draw red rectangle") {
		ColorRed red;
		color_red_init(&red);
		ShapeRectangle shape;
		shape_rectangle_init(&shape, &red.MCL_INTF(Color), 3, 4);
		ShapeInfo info = shape_draw(&shape.MCL_INTF(Shape));
		ASSERT_EQ(RECTANGLE, info.shape);
		ASSERT_EQ(RED, info.color);
		ASSERT_EQ(12, info.area);
	}

	TEST("should draw blue circle") {
		ColorBlue blue;
		color_blue_init(&blue);
		ShapeCircle shape;
		shape_circle_init(&shape, &blue.MCL_INTF(Color), 2);
		ShapeInfo info = shape_draw(&shape.MCL_INTF(Shape));
		ASSERT_EQ(CIRCLE, info.shape);
		ASSERT_EQ(BLUE, info.color);
		ASSERT_EQ(12, info.area);
	}
};
