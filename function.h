#pragma once
#include <raylib.h>

#include "types.h"


unsigned int eval(const Vector2 point, const unsigned int iterations) {
	int currIter = iterations;
	double re = point.x;
	double im = point.y;

	for (int i = 0; i < iterations; i++) {
		const double re2 = re * re;
		const double im2 = im * im;

		if (re2 + im2 > 4.0) {
			currIter = i;
			break;
		}

		// z = z² + c
		im = 2.0 * re * im + point.y;
		re = re2 - im2 + point.x;
	}

	return currIter == iterations ? 0 : currIter + 1;
}

