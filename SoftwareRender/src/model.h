#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <array>
#include "geometry.h"

struct Index {
	int index;
	int texCoordIndex;
	int nomalIndex;
};
using FaceIndex = std::array<Index, 3>;

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec2f> texCoord_;
	std::vector<Vec3f> nomal_;
	std::vector<FaceIndex> faces_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec2f texCoord(int i);
	Vec3f nomal(int i);
	FaceIndex face(int idx);
};

#endif //__MODEL_H__