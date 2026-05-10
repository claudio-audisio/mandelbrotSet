#pragma once
#include <chrono>
#include <cmath>
#include <raylib.h>
#include "cl_time.h"
#include "constants.h"
#include "function.h"


using namespace std;

class Board {
	float width, height;
	Vector2 center;
	bool haveToDraw = true;
	Image image;
	Color* pixels;
	Texture2D texture;

public:
	Board() = default;
	~Board() {
		free(pixels);
	};

	void init() {
		this->width = WIDTH;
		this->height = HEIGHT;
		center = Vector2(width / 2, height / 2);
		pixels = static_cast<Color*>(malloc(WIDTH * HEIGHT * sizeof(Color)));
		image = { pixels, WIDTH, HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
		texture = LoadTextureFromImage(image);
	}

	void draw() {
		if (haveToDraw) {
			ClearBackground(BG_COLOR);

			const auto start = std::chrono::steady_clock::now();

			for (int x = 0; x < WIDTH; x++) {
				for (int y = 0; y < HEIGHT; y++) {
					const Vector2 cartPoint = toCartesian(x, y);
					const unsigned int steps = eval(cartPoint, ITERATIONS);
					pixels[y * WIDTH + x] = getColor(steps);
				}
			}

			UpdateTexture(texture, pixels);
			DrawTexture(texture, 0, 0, WHITE);

			const auto time = cl::getElapsedMillis(start);
			cout << time << " ms" << endl;

			haveToDraw = false;
		}
	}

	void drawPoint(Vector2 point, int color) {
		DrawPixelV(point, RAYWHITE);
	}

	Vector2 toScreen(const Vector2 point) {
		return {
			roundf((point.x - getXOffset()) * 1000),
			roundf((point.y - getYOffset()) * 1000)
		};
	}

	static Vector2 toCartesian(const int x, const int y) {
		return {
			static_cast<float>(x + getXOffset()) / 1000,
			static_cast<float>(getYOffset() - y) / 1000
		};
	}

	static int getXOffset() {
		return -WIDTH + 1500;
	}

	static int getYOffset() {
		return HEIGHT / 2;
	}

	static Color getColor(const unsigned int steps) {
		if (steps == 0) {
			return BLACK;
		}

		return Color(
			static_cast<unsigned char>(steps % 256),
			static_cast<unsigned char>(steps * 2 % 256),
			static_cast<unsigned char>(steps * 3 % 256),
			255
			);
	}

};
