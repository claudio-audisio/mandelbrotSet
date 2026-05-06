#pragma once

#include "types.h"


#define FPS			12

// dimensions
#define WIDTH			4096
#define HEIGHT			2304
#define AXIS_THICK		1
#define POINT_RADIUS	3
#define FONT_SIZE		30

// colors
#define AXIS_COLOR			CLITERAL(Color){ 128, 128, 128, 255 }
#define BG_COLOR			CLITERAL(Color){ 31, 31, 31, 255 }
#define BG_COLOR_TRASP		CLITERAL(Color){ 31, 31, 31, 127 }
#define POINT_OUT_COLOR		ORANGE
#define POINT_IN_COLOR		GOLD

// mandelbrot
#define ITERATIONS		(128)
#define MAX_ITERATIONS	(1024)

// points of interest
const std::vector<Point> pointsOfInterest = {
	{-0.9915958639370167, -0.2999304239125738},
	{-0.042885704336861605, -0.6508349564225404},
	{-1.2408157693118302, -0.3238545956932012}
};