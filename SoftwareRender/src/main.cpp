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


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
std::shared_ptr<Model> model;
const int width = 800;
const int height = 800;
const int depth = 255;

Vec3f light = { 0.0, 0.0, -1.0 };
Vec3f camera = { 0.0, 0.0, 3.0 };


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
        
        
    }
    {
        Matrix projection = Matrix::identity();
        Matrix viewPort = GL::ViewPort(width / 8, height / 8, width *3/4, height * 3/4, depth );
        projection[3][2] = -1.f / camera.z;

        TGAImage texture;
        texture.read_tga_file("textures/african_head_diffuse.tga");
        texture.flip_vertically();

        TGAImage image(width, height, TGAImage::RGB);
        
      

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
                
                auto temp = GL::V4f_to_V3f(viewPort * projection * GL::V3f_to_V4f(world_coords[j], 1.0f));
                screen_coords[j] = { float(int(temp.x)), float(int(temp.y)), temp.z };  // today ,I spend most of my time to fix the bug!

            }
            Vec3f nomal = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
            float intensity = nomal.normalize() * light;
            if (intensity > 0)
            {
          
                GL::Triangle(screen_coords, zbufferf, image, texture_coords, texture, intensity);
            }
            
        }
       

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("outputs/perspective_project.tga");
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
        zbimage.write_tga_file("outputs/perspective_zbufferf.tga");
    }
    return 0;
}
