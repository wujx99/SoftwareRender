#pragma once
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"
#include <vector>
using TriIndex = Vec3i;

Vec3f BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
Vec3f V4f_to_V3f(Vec4f v);
Vec4f V3f_to_V4f(Vec3f v, float value);

struct BufferElemt
{
	Vec3f m_VertexCoord;
	Vec2f m_TextureCoord;
	Vec3f m_Normal;
};

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
	virtual void Triangle(int triIndex, TGAImage& image, TGAImage& zBuffer)  = 0;
	virtual DrawOutput glDraw(int width, int height)  = 0;
};



/*
这里的数据是相当于我们通过cpu传递的数据
*/
class Context
{
public:
	Context() = default;
	virtual ~Context() = default;

	inline void glBindVertexBuffer(const std::vector<BufferElemt>& vertexBuffer) { m_VertexBuffer = vertexBuffer; };
	void glSetVertArrayCoord();
	void glsetVertArrayTexCoord();
	void glSetVertArrayNormal();
	void glBindIndexBuffer(const std::vector<TriIndex>& indexBuffer) { m_IndexBuffer = indexBuffer; };
	inline void glBindTexture(const std::shared_ptr<TGAImage>& texture) { m_Texture = texture; };

	// get element
	inline Vec3f Vertex(int index) { return m_VertexArrayCoord[index]; };
	inline Vec2f TexCoord(int index) { return m_VertexArrayTexCoord[index]; };
	inline Vec3f Normal(int index) { return m_VertexArrayNormal[index]; };
	inline TriIndex TriangleIndex(int index) { return m_IndexBuffer[index]; };
	inline std::vector<TriIndex> IndexBuffer() { return m_IndexBuffer; };

	

private:

	std::vector<BufferElemt> m_VertexBuffer;
	std::vector<Vec3f> m_VertexArrayCoord;
	std::vector<Vec2f> m_VertexArrayTexCoord;
	std::vector<Vec3f> m_VertexArrayNormal;
	std::vector<TriIndex> m_IndexBuffer;
	std::shared_ptr<TGAImage> m_Texture;

	int m_width{800}, m_heigth{800};
};