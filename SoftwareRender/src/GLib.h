#pragma once
#include "geometry.h"
#include "tgaimage.h"

struct DrawOutput
{
	TGAImage drawedImage;
	TGAImage zbuffer;
};
/*
派生类添加的数据相当于是glsl的数据
*/
struct Shader
{
	virtual ~Shader() = default;
	virtual Vec3f Vertex(int triIndex, int vertIndex) = 0;
	virtual bool Fragment(Vec3f baryCoord, TGAColor& color) = 0;
	virtual DrawOutput glDraw(int width, int height) = 0;
	void Triangle(int triIndex, TGAImage& image, TGAImage& zBuffer);
};
