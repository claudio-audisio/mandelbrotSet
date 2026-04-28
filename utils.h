#pragma once
#include <chrono>

#include "types.h"

using namespace std;

inline unsLL getElapsedNanos(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedMicros(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedMillis(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
}

inline unsLL getElapsedSeconds(const chrono::time_point<chrono::steady_clock> start) {
	return chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
}