#pragma once
#include "geometry.h"
#include "tgaimage.h"


/*
��������ӵ������൱����glsl������
*/
struct Shader
{
	virtual ~Shader() = default;
	virtual Vec3f Vertex(int triIndex, int vertIndex) = 0;
	virtual bool Fragment(Vec3f baryCoord, TGAColor& color) = 0;
	static void Triangle(int triIndex, Shader& shader, TGAImage& image, TGAImage& zBuffer) ;
	
};
