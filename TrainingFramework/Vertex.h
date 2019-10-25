#pragma once
#include "../Utilities/MyMath.h"

#define OFFSET_POS 0
#define OFFSET_NORM 12
#define OFFSET_BINORM 24
#define OFFSET_TGT 36
#define OFFSET_UV 48

struct Vertex 
{
	Vector3 pos;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tangent;
	Vector2 uv;
};