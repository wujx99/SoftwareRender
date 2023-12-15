#pragma once
#pragma once
#include "geometry.h"
struct Tools
{
	static Vec3f BaryCentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P);

	static Vec3f BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
	static Matrix ViewPort(int x, int y, int w, int h, int depth);
	static Matrix Projection(float coeff = 0.f);
	static Vec3f V4f_to_V3f(Vec4f v);
	static Vec4f V3f_to_V4f(Vec3f v, float value);

	static Matrix ULookUp(Vec3f center, Vec3f eye, Vec3f up);
};