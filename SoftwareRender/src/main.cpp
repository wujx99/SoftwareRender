#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"
#include "GL.h"
#include "GLib.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <optional>
#include <vector>
#include <array>
#include <memory>

#include "tests/TestGL.h"
struct DrawOutput
{
    TGAImage drawedImage;
    TGAImage zbuffer;
};
DrawOutput glDraw(int width, int height)
{
    TGAImage retImage(width, height, TGAImage::RGB);
    TGAImage retZBuffer(width, height, TGAImage::GRAYSCALE);

    int size = m_Context.IndexBuffer().size();
    for (int i = 0; i < size; i++) {
        Triangle(i, retImage, retZBuffer);
    }
    return { retImage, retZBuffer };
}

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

    std::vector<float> zbufferf(width * height, std::numeric_limits<float>::min());

    //tests
    {
        test::TestGL::TestBaryCentric();
        
        test::TestGL::TestULooKUp();
    }
    {
        Matrix modelView = GL::ULookUp(center, eye, Vec3f(0, 1, 0));
        Matrix projection = Matrix::identity();
        Matrix viewPort = GL::ViewPort(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
        projection[3][2] = -1.f / (eye - center).norm();

        std::cerr << modelView << std::endl;
        std::cerr << projection << std::endl;
        std::cerr << viewPort << std::endl;
        Matrix z = (viewPort * projection * modelView);
        std::cerr << z << std::endl;

        /*TGAImage texture;
        texture.read_tga_file("textures/african_head_diffuse.tga");
        texture.flip_vertically();*/
        
        TGAImage texture(width, height, TGAImage::RGB);
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                texture.set(i, j, TGAColor(255, 255, 255));
        

        TGAImage image(width, height, TGAImage::RGB);
        
        int nonDrawcount = 0, drawCount = 0;

        for (int i = 0; i < model->nfaces(); i++) {
            FaceIndex face = model->face(i);
            std::array<Vec3f, 3> screen_coords;
            std::array<Vec2f, 3> texture_coords;
            std::array<Vec3f, 3> world_coords;
            std::array<float, 3> vertex_intensity;
            for (int j = 0; j < 3; j++) {
                Vec3f v = model->vert(face[j].index);
                Vec2f vt = model->texCoord(face[j].texCoordIndex);
                Vec3f vn = model->nomal(face[j].nomalIndex);
                world_coords[j] = v;
                texture_coords[j] = vt;
                vertex_intensity[j] = vn.normalize() * light_dir;
                auto temp = GL::V4f_to_V3f(viewPort * projection * modelView * GL::V3f_to_V4f(world_coords[j], 1.0f));
                screen_coords[j] = { float(int(temp.x)), float(int(temp.y)), temp.z };  // today ,I spend most of my time to fix this bug!

            }

            /*
            something goes wrong! i change the direction of nomal to fix!!!!
            */
            Vec3f nomal = cross(world_coords[1] - world_coords[0], world_coords[2] - world_coords[0]);
            float intensity = nomal.normalize() * light_dir;
            
            if (intensity > 0)
            {
                drawCount++;
                GL::Triangle(screen_coords, zbufferf, image, texture_coords, texture, vertex_intensity);
            }
            else
            {
                nonDrawcount++;
            }
            
        }
        std::cout << "draw count :" << drawCount << "non draw count :" << nonDrawcount <<  std::endl;

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("outputs/Gouraudshading.tga");
    }
    {
        // dump zbuffer for debug
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                zbimage.set(i, j, TGAColor(zbufferf[i + j * width], 1));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("outputs/GouraudshadingZbuffer.tga");
    }

    {
        struct GouraudShader :public Shader
        {





            virtual Vec3f Vertex(int triIndex, int vertIndex) override
            {
                m_TriVertIntensity[vertIndex] = m_Context.Normal(m_Context.TriangleIndex(triIndex)[vertIndex]) * m_LightDir;
                Vec3f gl_Vertex = m_Context.Vertex(m_Context.TriangleIndex(triIndex)[vertIndex]);
                return V4f_to_V3f(m_ViewPort * m_Proj * m_Model * embed<4>(gl_Vertex));
            }
            virtual bool Fragment(Vec3f baryCoord, TGAColor& color) override
            {
                float intensity = m_TriVertIntensity * baryCoord;
                color = TGAColor(255, 255, 255) * intensity;
                return false;
            }
        }
    }
    return 0;
}
