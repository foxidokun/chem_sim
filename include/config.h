#pragma once

typedef unsigned int uint;

#define ALWAYS_SAME_RANDOM 0

const char *const WINDOW_TITLE   = "Render preview";
const double NEAR_ZERO_VEC_1DLEN = 1e-5;

const uint WINDOW_WIDTH  = 1080;
const uint WINDOW_HEIGHT = 720;

const uint PISTON_HEIGHT = 5;

const double BASE_RADIUS       = 1;
const double MASS_RADIUS_COEFF = 0.2;

const double GC_THRESHOLD = 0.1;