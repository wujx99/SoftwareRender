#pragma once
#include <optional>
#include <array>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

class Tools
{
public:
	Tools() = default;
	~Tools() = default;

	static Vec3f BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

	static void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color);
	static void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
	static void triangle(const std::array<Vec3f, 3>& pts,
		std::vector<float>& zbuffer, TGAImage& image, TGAColor color);
	static Vec3f world2screen(Vec3f v, int width, int height);
};

