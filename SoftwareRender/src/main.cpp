#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"

#include <algorithm>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);


Model* model = NULL;
const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x1 < x0) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}
	}
}

void line(Vec2i v0, Vec2i v1, TGAImage& image, TGAColor color) {
	line(v0.x, v0.y, v1.x, v1.y, image, color);
}

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage& image, TGAColor color) {
	line(v0, v1, image, color);
	line(v1, v2, image, color);
	line(v2, v0, image, color);	
}

int calcCorrespondX(Vec2i start, Vec2i end, int y)
{
	if (end.x == start.x) return start.x;
	if (end.y == start.y) return end.x;

	float slope = (end.y - start.y) / (float)(end.x - start.x);
	int res = (int)((y - start.y) / slope + start.x);
	return res;
}

void filledtriangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage& image, TGAColor color) {
	std::vector<Vec2i> vexArray = { v0, v1, v2 };
	std::sort(vexArray.begin(), vexArray.end(), [](Vec2i a, Vec2i b){return a.y < b.y;});
	Vec2i left = vexArray[1], right = vexArray[2];
	if (left.x > right.x) {
		std::swap(left, right);
	}
	SR_ASSERT(left.x < right.x, "the judge of left and right vextex of triangle wrong!")
	auto bottom = vexArray[0], top = vexArray[2];

	enum class TriangleType
	{
		NONE,TOP_FLAT,BOTTOM_FLAT, LEFT_SHORT, RIGHT_SHORT
	};
	TriangleType tritype;
	
	if (left.y < right.y) 
	{ 
		tritype = TriangleType::LEFT_SHORT; 
	}
	if (left.y > right.y) 
	{
		tritype = TriangleType::RIGHT_SHORT;
	}
	if (bottom.y == left.y || bottom.y == right.y) 
	{
		tritype = TriangleType::BOTTOM_FLAT;
	}
	if (left.y == right.y) 
	{
		tritype = TriangleType::TOP_FLAT;
	}

	switch (tritype)
	{
	case TriangleType::TOP_FLAT:
		for (int y = bottom.y; y <= top.y; y++) {
			int start_x = calcCorrespondX(bottom, left, y);
			int end_x = calcCorrespondX(bottom, right, y);
			Vec2i start = { start_x, y }, end = { end_x, y };
			line(start, end, image, color);
		}
		break;
	case TriangleType::BOTTOM_FLAT:
		for (int y = bottom.y; y <= top.y; y++) {
			int start_x = std::min(calcCorrespondX(bottom, left, y), calcCorrespondX(left, right, y));
			int end_x = std::max(calcCorrespondX(bottom, left, y), calcCorrespondX(left, right, y));
			Vec2i start = { start_x, y }, end = { end_x, y };
			line(start, end, image, color);
		}
		break;
	case TriangleType::LEFT_SHORT:
		for (int y = bottom.y; y <= top.y; y++) {
			int start_x, end_x;
			if (y < left.y) {
				start_x = calcCorrespondX(bottom, left, y);
				end_x = calcCorrespondX(bottom, right, y);
			}
			else {
				start_x = calcCorrespondX(left, top, y);
				end_x = calcCorrespondX(bottom, right, y);
			}
			
			Vec2i start = { start_x, y }, end = { end_x, y };
			line(start, end, image, color);
		}
		break;
	case TriangleType::RIGHT_SHORT:
		for (int y = bottom.y; y <= top.y; y++) {
			int start_x, end_x;
			if (y < right.y) {
				start_x = calcCorrespondX(bottom, left, y);
				end_x = calcCorrespondX(bottom, right, y);
			}
			else {
				start_x = calcCorrespondX(bottom, left, y);
				end_x = calcCorrespondX(right, top, y);
			}

			Vec2i start = { start_x, y }, end = { end_x, y };
			line(start, end, image, color);
		}
		break;
	default:
		break;
	}
	
}
	

int main(int argc, char** argv) {
	SR::Log::Init();
	TGAImage image(width, height, TGAImage::RGB);
	Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
	filledtriangle(t0[0], t0[1], t0[2], image, red);
	filledtriangle(t1[0], t1[1], t1[2], image, white);
	filledtriangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("triangle.tga");
	delete model;
	return 0;
}