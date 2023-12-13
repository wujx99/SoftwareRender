#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"
#include "GL.h"

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

/*
this function look at is from tutorial, I think is wrong!!!
my function is GL::ULookAT.
*/
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up , z).normalize();
    Vec3f y = cross(z , x).normalize();
    Matrix res = Matrix::identity();
    for (int i = 0; i < 3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
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
        Matrix modelView2 = lookat(eye, center, Vec3f(0, 1, 0));
        Matrix projection = Matrix::identity();
        Matrix viewPort = GL::ViewPort(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
        projection[3][2] = -1.f / (eye - center).norm();

        std::cerr << modelView << std::endl;
        std::cerr << modelView2 << std::endl;
        std::cerr << projection << std::endl;
        std::cerr << viewPort << std::endl;
        Matrix z = (viewPort * projection * modelView);
        std::cerr << z << std::endl;

        TGAImage texture;
        texture.read_tga_file("textures/african_head_diffuse.tga");
        texture.flip_vertically();

        TGAImage image(width, height, TGAImage::RGB);
        
        int nonDrawcount = 0, drawCount = 0;

        for (int i = 0; i < model->nfaces(); i++) {
            FaceIndex face = model->face(i);
            std::array<Vec3f, 3> screen_coords;
            std::array<Vec2f, 3> texture_coords;
            std::array<Vec3f, 3> world_coords;
            for (int j = 0; j < 3; j++) {
                Vec3f v = model->vert(face[j].index);
                Vec2f vt = model->texCoord(face[j].texCoordIndex);
                world_coords[j] = v;
                texture_coords[j] = vt;
                
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
                GL::Triangle(screen_coords, zbufferf, image, texture_coords, texture, intensity);
            }
            else
            {
                nonDrawcount++;
            }
            
        }
        std::cout << "draw count :" << drawCount << "non draw count :" << nonDrawcount <<  std::endl;

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("outputs/my_camera.tga");
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
        zbimage.write_tga_file("outputs/camera_zbufferf.tga");
    }
    return 0;
}
