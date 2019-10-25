#pragma once
#define _USE_MATH_DEFINES
#include <math.h>


class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;
	static int keyPressed;
};

#define MOUSE_DOWN 0
#define MOUSE_UP 1
#define MOUSE_MOVE 2

#define MAX_STRING_LENGTH 256
#define RESOURCE_PATH "../Resources/"
#define MAX_PITCH_ANGLE TO_RAD(80)
#define TO_RAD(degree) ((degree) * M_PI / 180)

#define MOVE_FORWARD	1
#define MOVE_BACK		1 << 1
#define MOVE_LEFT		1 << 2
#define MOVE_RIGHT		1 << 3
#define ROTATE_LEFT		1 << 4
#define ROTATE_RIGHT	1 << 5
#define ROTATE_UP		1 << 6
#define ROTATE_DOWN		1 << 7
