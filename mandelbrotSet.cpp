#include <iostream>
#include <raylib.h>

#include "board.h"
#include "constants.h"

Board board;
Shader shader;
int offsetLoc, zoomLoc, maxIterLoc, resolutionLoc;
Vector2 offset;
float zoom;
int iterations;
const Vector2 resolution = { static_cast<float>(WIDTH), static_cast<float>(HEIGHT) };
bool haveToDraw;

void resetPosition() {
	offset = {-0.5f, 0.0f};
	zoom = 0.35f;
	iterations = ITERATIONS;
	haveToDraw = true;
}

void init() {
	SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
	InitWindow(WIDTH, HEIGHT, "mandelbrot set");
	SetTargetFPS(FPS);
	SetWindowPosition(0, 0);

	//board.init();

	// Load a fragment shader (vertex shader can be NULL for default)
	shader = LoadShader(nullptr, "../mandelbrot.frag");

	// Get locations of uniform variables
	offsetLoc = GetShaderLocation(shader, "offset");
	zoomLoc   = GetShaderLocation(shader, "zoom");
	maxIterLoc = GetShaderLocation(shader, "maxIter");
	resolutionLoc = GetShaderLocation(shader, "resolution");

	resetPosition();
}

void zoomControl() {
	const float scroll = GetMouseWheelMove();

	if (scroll != 0) {
		const Vector2 mouse = GetMousePosition();
		const float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

		// Mouse in complex plane before zoom
		const float mx = ((mouse.x / WIDTH  - 0.5f) * aspect) / zoom + offset.x;
		const float my = ((1 - mouse.y / HEIGHT - 0.5f)) / zoom + offset.y;

		// Zoom
		zoom *= (scroll > 0) ? 1.2f : 0.8f;

		// Recalculate offset so the same complex point stays under mouse
		offset.x = mx - ((mouse.x / WIDTH  - 0.5f) * aspect) / zoom;
		offset.y = my - ((1 - mouse.y / HEIGHT - 0.5f)) / zoom;

		haveToDraw = true;
	}
}

void panControl() {
	const float speed = 0.01f / zoom;

	if (IsKeyDown(KEY_LEFT)) {
		offset.x -= speed; haveToDraw = true;
	}

	if (IsKeyDown(KEY_RIGHT)) {
		offset.x += speed; haveToDraw = true;
	}

	if (IsKeyDown(KEY_UP)) {
		offset.y += speed; haveToDraw = true;
	}

	if (IsKeyDown(KEY_DOWN)) {
		offset.y -= speed; haveToDraw = true;
	}

	if (IsKeyPressed(KEY_R)) {
		resetPosition();
	}

	iterations = min(static_cast<int>(ITERATIONS + max(ITERATIONS * log2f(zoom), 0.0f)), MAX_ITERATIONS);
}

int main() {
	init();

	while (!WindowShouldClose()) {
		zoomControl();
		panControl();

		SetShaderValue(shader, offsetLoc, &offset, SHADER_UNIFORM_VEC2);
		SetShaderValue(shader, zoomLoc,	&zoom, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, maxIterLoc, &iterations, SHADER_UNIFORM_INT);
		SetShaderValue(shader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);

		BeginDrawing();

		if (haveToDraw) {
			ClearBackground(BG_COLOR);

			BeginShaderMode(shader);
			DrawRectangle(0, 0, WIDTH, HEIGHT, WHITE);
			EndShaderMode();
			//board.draw();

			DrawText(format("{}", iterations).c_str(), 50, 50, 20, RAYWHITE);
			haveToDraw = false;
		}

		EndDrawing();
	}

	CloseWindow();

    return 0;
}
