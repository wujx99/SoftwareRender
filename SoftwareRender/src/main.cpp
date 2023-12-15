#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"
#include "GLib.h"
#include "Tools.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <array>
#include <memory>



const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
std::shared_ptr<Model> model;
const int width = 800;
const int height = 800;
const int depth = 255;

Vec3f light_dir = Vec3f(1, -1, 1).normalize();
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);


int main(int argc, char** argv) {
    if (2 == argc) {
        model = std::make_shared<Model>(argv[1]);
    }
    else {
        model = std::make_shared<Model>("obj/african_head.obj");
    }

    {
        struct GouraudShader :public Shader
        {

            virtual Vec3f Vertex(int triIndex, int nthIndex) override
            {
                int vertIndex = s_Model->face(triIndex)[nthIndex].index;
                int normalIndex = s_Model->face(triIndex)[nthIndex].nomalIndex;
                Vec3f vertexCoord = s_Model->vert(vertIndex);
                Vec3f normalCoord = s_Model->nomal(normalIndex);

                Vec3f gl_Vertex = Tools::V4f_to_V3f(m_ViewPort * m_Proj * m_View * m_Model * embed<4>(vertexCoord));
                m_TriVertIntensity[nthIndex] = normalCoord * m_LightDir;

                return gl_Vertex;
            }
            virtual bool Fragment(Vec3f baryCoord, TGAColor& color) override
            {
                float intensity = m_TriVertIntensity * baryCoord;
                color = TGAColor(255, 255, 255) * intensity;
                return false;
            }
            virtual DrawOutput glDraw(int width, int height) override
            {
                TGAImage retImage(width, height, TGAImage::RGB);
                TGAImage retZBuffer(width, height, TGAImage::GRAYSCALE);

                int size = s_Model->nfaces();
                for (int i = 0; i < size; i++) {
                    Triangle(i, retImage, retZBuffer);
                }
                return { retImage, retZBuffer };
            }

        public:
            // cpu side data
            std::shared_ptr<Model> s_Model;
        private:
            // for shaders states
            Vec3f m_TriVertIntensity{};
        public:
            // uniform
            Vec3f m_LightDir{};
            Matrix m_Model{ Matrix::identity() };
            Matrix m_View{ Matrix::identity() };
            Matrix m_Proj{ Matrix::identity() };
            Matrix m_ViewPort{ Matrix::identity() };
        };

        GouraudShader myShader;

        myShader.m_LightDir = light_dir;
        myShader.s_Model = model;
        myShader.m_View = Tools::ULookUp(center, eye, Vec3f(0, 1, 0));
        myShader.m_Proj = Tools::Projection(-1.f / (eye - center).norm());
        myShader.m_ViewPort = Tools::ViewPort(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth); //depthh must set to 255;

        auto retimage = myShader.glDraw(width, height);

        retimage.drawedImage.flip_vertically(); // to place the origin in the bottom left corner of the image
        retimage.zbuffer.flip_vertically();
        retimage.drawedImage.write_tga_file("outputs/gshader2.tga");
        retimage.zbuffer.write_tga_file("outputs/gshaderzbuffer2.tga");

    }
    return 0;
}
