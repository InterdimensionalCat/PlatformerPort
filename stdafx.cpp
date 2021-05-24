#include "stdafx.h"

constexpr float METER_TO_PIXEL = 64.0;
float toPixels(const float meters) {
	return meters * METER_TO_PIXEL;
}

float toMeters(const float pixels) {
	return pixels / METER_TO_PIXEL;
}