#pragma once

typedef unsigned int uint;

#define ALWAYS_SAME_RANDOM 0
#define DEBUG_CHECK_MASS   0

const char *const WINDOW_TITLE   = "Render preview";
const double NEAR_ZERO_VEC_1DLEN = 1e-5;

const uint WINDOW_WIDTH  = 1080;
const uint WINDOW_HEIGHT = 720;

const uint GAS_WIDTH  = 409;
const uint GAS_HEIGHT = 646;

const double GAS_POS[2] = {41, 38};

const uint PISTON_HEIGHT = 5;

const double BASE_RADIUS       = 3;
const double MASS_RADIUS_COEFF = 0.5;

const double MIN_VELOCITY = 0.05;

const char BACKGROUNG_PATH[] = "../textures/background.png"; 