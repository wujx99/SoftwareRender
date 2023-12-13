#pragma once
#include <optional>
#include <array>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

class GL
{
public:
	GL() = default;
	~GL() = default;

	static Vec3f BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
	static Matrix ViewPort(int x, int y, int w, int h, int depth);
	static Vec3f V4f_to_V3f(Vec4f v);
	static Vec4f V3f_to_V4f(Vec3f v, float value);

	static Matrix ULookUp(Vec3f center, Vec3f eye, Vec3f up);

	static void Line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color);
	static void Triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
	static void Triangle(const std::array<Vec3f, 3>& pts,
		std::vector<float>& zbuffer, TGAImage& image, TGAColor color);
	static Vec3f World2Screen(Vec3f v, int width, int height);

	/*
	the vertex of triangle "pts", vextex.x and vertex.y must have the form .f,
	means the int form "float"
	*/
	static void Triangle(const std::array<Vec3f, 3>& pts, std::vector<float>& zbuffer,
		TGAImage& image, const std::array<Vec2f, 3>& texCoord, TGAImage& texture, float intensity = 1.0f);
};

