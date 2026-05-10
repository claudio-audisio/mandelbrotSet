#include <iostream>

#include "board.h"
#include "constants.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include "raylib.h"
#include "rlgl.h"

using namespace std;


Board board;
Shader shader;
Font font;
int offsetLoc, zoomLoc, maxIterLoc, resolutionLoc;
double offset[2];
double zoom;
double mx, my;
int targetIterations;
int currentIterations;
constexpr double resolution[2] = { static_cast<double>(WIDTH), static_cast<double>(HEIGHT) };
constexpr double aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
bool drawPts;
int autoZoom;

void resetIterations() {
	currentIterations = ITERATIONS;
	targetIterations = ITERATIONS;
}

void resetPosition() {
	offset[0] = -0.5;
	offset[1] = 0.0;
	zoom = 0.35;
	resetIterations();
	drawPts = false;
	autoZoom = 0;
	ShowCursor();
}

void init() {
	SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
	InitWindow(WIDTH, HEIGHT, "mandelbrot set");
	TraceLog(LOG_INFO, "OpenGL version: %s", glGetString(GL_VERSION));
	TraceLog(LOG_INFO, "GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	SetTargetFPS(FPS);
	SetWindowPosition(0, 0);
	SetWindowFocused();

	// Load a fragment shader (vertex shader can be NULL for default)
	shader = LoadShader(nullptr, "../mandelbrot.frag");

	// Get locations of uniform variables
	offsetLoc     = glGetUniformLocation(shader.id, "offset");
	zoomLoc       = glGetUniformLocation(shader.id, "zoom");
	maxIterLoc    = glGetUniformLocation(shader.id, "maxIter");
	resolutionLoc = glGetUniformLocation(shader.id, "resolution");

	font = LoadFontEx("/home/claudio/Projects/cartesio/asset/SourceCodePro-Regular.ttf", FONT_SIZE, nullptr, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

	resetPosition();
}

int computeTargetIterations(const double zoom) {
	return min(static_cast<int>(ITERATIONS + max(25.0 * log2(zoom), 0.0)), MAX_ITERATIONS);
}

Vector2 complexToScreen(const Vector2 cPoint) {
	return {
		static_cast<float>(((cPoint.x - offset[0]) * zoom / aspect + 0.5) * WIDTH),
		static_cast<float>((1.0 - ((cPoint.y - offset[1]) * zoom + 0.5)) * HEIGHT)
	};
}

void launchAutoZoom() {
	HideCursor();
	SetMousePosition(WIDTH / 2, HEIGHT / 2);
	mx = pointsOfInterest[4].x;
	my = pointsOfInterest[4].y;
	autoZoom = 1;
}

void zoomControl(float scroll = 0) {
	const Vector2 mouse = GetMousePosition();

	if (scroll == 0) {
		scroll = GetMouseWheelMove();
		mx = ((mouse.x / WIDTH  - 0.5f) * aspect) / zoom + offset[0];
		my = ((1 - mouse.y / HEIGHT - 0.5f)) / zoom + offset[1];
	}

	if (scroll != 0) {
		zoom *= (scroll > 0) ? 1.05 : 0.95;

		// Recalculate offset so the same complex point stays under mouse
		offset[0] = mx - ((mouse.x / WIDTH  - 0.5f) * aspect) / zoom;
		offset[1] = my - ((1 - mouse.y / HEIGHT - 0.5f)) / zoom;

		targetIterations = computeTargetIterations(zoom);

		if (targetIterations >= MAX_ITERATIONS) {
			autoZoom = 0;
			ShowCursor();
		}
	}

	if (IsKeyPressed(KEY_R)) resetPosition();
	if (IsKeyDown(KEY_KP_ADD)) zoom *= 1.2;
	if (IsKeyDown(KEY_KP_SUBTRACT)) zoom *= 0.8;
	if (IsKeyPressed(KEY_M)) targetIterations = 1024;
	if (IsKeyPressed(KEY_C)) resetIterations();
	if (IsKeyPressed(KEY_Z)) launchAutoZoom();
	if (IsKeyPressed(KEY_I)) targetIterations *= 2;
}

void panControl() {
	const double speed = 0.05 / zoom;

	if (IsKeyDown(KEY_LEFT)) offset[0] -= speed;
	if (IsKeyDown(KEY_RIGHT)) offset[0] += speed;
	if (IsKeyDown(KEY_UP)) offset[1] += speed;
	if (IsKeyDown(KEY_DOWN)) offset[1] -= speed;
}

void pointControl() {
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		drawPts = true;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		drawPts = false;
		cout << format("{} {}", mx, my) << endl;
	}
}

// TODO rendere piu' fluido il tutto: dovrei usare il doppio double (vedi Claude)
// aggiungere tooltip dei punti
// aggiungere la successione dei punti (limitata ?)

void drawInfoBox() {
	DrawRectangle(0, 0, 200, 110, BG_COLOR_TRASP);
	DrawTextEx(font, format("{}", currentIterations).c_str(), {20, 20}, FONT_SIZE, 1, RAYWHITE);
	DrawTextEx(font, format("{:g}", zoom).c_str(), {20, 60}, FONT_SIZE, 1, RAYWHITE);
	//DrawTextEx(font, format("{} {}", mx, my).c_str(), {20, 100}, FONT_SIZE, 1, RAYWHITE);
}

vector<Vector2> calculatePoints(const float x, const float y, const int maxIter) {
	vector<Vector2> points;
	Vector2 c = {x, y};
	Vector2 z = {0, 0};

	for (int i = 0; i < maxIter; i++) {
		if (z.x * z.x + z.y * z.y > 4.0) {
			break;
		}

		z = Vector2(z.x * z.x - z.y * z.y + c.x, 2.0 * z.x * z.y + c.y);

		points.push_back(complexToScreen(z));
	}

	return points;
}

void drawPoints() {
	if (drawPts) {
		const vector<Vector2> points = calculatePoints(mx, my, targetIterations);
		const Vector2 pos = GetMousePosition();
		Color POINT_COLOR = POINT_IN_COLOR;

		if (points.size() != targetIterations) {
			POINT_COLOR = POINT_OUT_COLOR;
			DrawTextEx(font, format("{:.4f} {:.4f} ({})", mx, my, points.size()).c_str(), {pos.x + 10, pos.y - 30}, FONT_SIZE, 1, POINT_COLOR);
		} else {
			DrawTextEx(font, format("{:.4f} {:.4f}", mx, my).c_str(), {pos.x + 10, pos.y - 30}, FONT_SIZE, 1, POINT_COLOR);
		}

		Vector2 previous = points[0];
		DrawCircleV(points[0], POINT_RADIUS, POINT_COLOR);

		for (int i = 1; i < points.size(); i++) {
			DrawCircleV(points[i], POINT_RADIUS, POINT_COLOR);
			DrawLineV(previous, points[i], POINT_COLOR);
			previous = points[i];
		}
	}
}

int main() {
	init();

	while (!WindowShouldClose()) {
		zoomControl(autoZoom);
		panControl();
		pointControl();

		if (currentIterations < targetIterations) {
			currentIterations += 25;  // increase by 25 each frame
			currentIterations = min(currentIterations, targetIterations);
		}

		glUseProgram(shader.id);
		glUniform2d(offsetLoc, offset[0], offset[1]);
		glUniform1d(zoomLoc, zoom);
		glUniform2d(resolutionLoc, resolution[0], resolution[1]);
		glUniform1i(maxIterLoc, currentIterations);

		BeginDrawing();
			ClearBackground(BG_COLOR);

			BeginShaderMode(shader);
			DrawRectangle(0, 0, WIDTH, HEIGHT, WHITE);
			EndShaderMode();

			drawInfoBox();
			drawPoints();
		EndDrawing();
	}

	CloseWindow();

    return 0;
}
