#include "GLib.h"
#include "Tools.h"
#include <array>
void Shader::Triangle(int triIndex, Shader& shader, TGAImage& image, TGAImage& zBuffer)  
{
    const int width = image.get_width(), height = image.get_height();
    std::array<Vec3f, 3> pts = { shader.Vertex(triIndex, 0), shader.Vertex(triIndex, 1), shader.Vertex(triIndex, 2) };


    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec2i p;
    TGAColor color;
    float z = 0;
    for (p.x = bboxmin.x; p.x < bboxmax.x; p.x++) {
        for (p.y = bboxmin.y; p.y < bboxmax.y; p.y++) {
            auto barycoord = Tools::BaryCentric(proj<2>(pts[0]), proj<2>(pts[1]), proj<2>(pts[2]), Vec2f(float(p.x), float(p.y)));//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////fromtheisiisisisisid
            for (int i = 0; i < 3; i++)
            {
                z += barycoord[i] * pts[i].z;
            }
            int frag_depth = std::max(0, std::min(255, int(z + .5)));
            if (barycoord.x < 0 || barycoord.y < 0 || barycoord.z < 0 || zBuffer.get(p.x, p.y)[0]>frag_depth) continue;
            bool discard = shader.Fragment(barycoord, color);
            if (!discard) {
                zBuffer.set(p.x, p.y, TGAColor(frag_depth));
                image.set(p.x, p.y, color);
            }

        }
    }
}
