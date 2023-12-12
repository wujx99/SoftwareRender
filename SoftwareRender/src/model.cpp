#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }
        else if(!line.compare(0, 3, "vt "))
        {
            iss >> trash >> trash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt[i];
            texCoord_.push_back(vt);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash >> trash;
            Vec3f vn;
            for (int i = 0; i < 3; i++) iss >> vn[i];
            nomal_.push_back(vn);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::array<Index, 3> f;
            int  idx, texIndex, nomalIndex;
            iss >> trash;
            int pushIndex = 0;
            while (iss >> idx >> trash >> texIndex >> trash >> nomalIndex) {
                idx--; texIndex--, nomalIndex--;// in wavefront obj all indices start at 1, not zero
                    f[pushIndex++] = { idx, texIndex, nomalIndex };
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " <<texCoord_.size() << "vn#" << nomal_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

FaceIndex Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::texCoord(int i)
{
    return texCoord_[i];
}

Vec3f Model::nomal(int i)
{
    return nomal_[i];
}
