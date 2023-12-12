#include "Tools.h"



Vec3f Tools::BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f AB = B - A, AC = C - A, PA = A - P;
    Vec3f vec1{ AB.x, AC.x ,PA.x }, vec2{ AB.y, AC.y, PA.y };
    Vec3f uv = cross(vec1, vec2);
    if (std::abs(uv[2]) > 1e-2)
        return { 1.f - (uv.x + uv.y) / uv.z, uv.x / uv.z, uv.y / uv.z };
    return {-1, 1, 1};
}

void Tools::line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x > p1.x) {
        std::swap(p0, p1);
    }

    for (int x = p0.x; x <= p1.x; x++) {
        float t = (x - p0.x) / (float)(p1.x - p0.x);
        int y = p0.y * (1.f - t) + p1.y * t;
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
    }
}

void Tools::triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    if (t0.y == t1.y && t0.y == t2.y) return; // i dont care about degenerate triangles
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}


Vec3f Tools::world2screen(Vec3f v, int width, int height) {
    Vec3f ret = Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
    return ret;
}

void Tools::triangle(const std::array<Vec3f, 3>& pts, std::vector<float>& zbuffer, TGAImage& image, TGAColor color) {
   
    const int width = image.get_width(), height = image.get_height();
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f p;
    for (p.x = bboxmin.x; p.x < bboxmax.x; p.x++) {
        for (p.y = bboxmin.y; p.y < bboxmax.y; p.y++) {
            auto barycoord = Tools::BaryCentric(pts[0], pts[1], pts[2], p);
            //std::cout << barycoord << "barycoord" << std::endl;

            if (barycoord.x < 0 || barycoord.y < 0 || barycoord.z < 0) continue;
            p.z = 0;
            for (int i = 0; i < 3; i++) p.z += barycoord[i] * pts[i].z;

            if (zbuffer[int(p.x + p.y * width)] < p.z) {
                zbuffer[int(p.x + p.y * width)] = p.z;
                //std::cout << "set x" << p.x << " y" << p.x << std::endl;
                image.set(p.x, p.y, color);
            }

        }
    }
}