#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"
#include "Tools.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <optional>
#include <vector>
#include <array>

#include "tests/TestTools.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;



int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
        //model = new Model("obj/test.obj");
    }

    //tests
    {
        test::TestTools::TestBaryCentric();
    }

    TGAImage texture;
    texture.read_tga_file("textures/african_head_diffuse.tga");
    texture.flip_vertically();


    std::vector<float> zbuffer(width * height, -std::numeric_limits<float>::max());
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
            screen_coords[j] = Tools::world2screen(world_coords[j], width, height);
            
        }
        
        Tools::triangle(screen_coords,zbuffer, image, texture_coords, texture);
       
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("outputs/texture.tga");
    delete model;
    return 0;
}
