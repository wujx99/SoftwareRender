#include "Tools.h"

Vec3f Tools::BaryCentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec2f AB = B - A, AC = C - A, PA = A - P;
    Vec3f vec1{ AB.x, AC.x ,PA.x }, vec2{ AB.y, AC.y, PA.y };
    Vec3f uv = cross(vec1, vec2);
    if (std::abs(uv[2]) > 1e-2)
        return { 1.f - (uv.x + uv.y) / uv.z, uv.x / uv.z, uv.y / uv.z };
    return { -1, 1, 1 };
}

Vec3f Tools::BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f AB = B - A, AC = C - A, PA = A - P;
    Vec3f vec1{ AB.x, AC.x ,PA.x }, vec2{ AB.y, AC.y, PA.y };
    Vec3f uv = cross(vec1, vec2);
    if (std::abs(uv[2]) > 1e-2)
        return { 1.f - (uv.x + uv.y) / uv.z, uv.x / uv.z, uv.y / uv.z };
    return { -1, 1, 1 };
}

Matrix Tools::ViewPort(int x, int y, int w, int h, int depth) {
    Matrix m = Matrix::identity();
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}

Vec3f Tools::V4f_to_V3f(Vec4f v) {
    return { v[0] / v[3], v[1] / v[3],v[2] / v[3] };
}
Vec4f Tools::V3f_to_V4f(Vec3f v, float value)
{
    Vec4f ret;
    ret[0] = v.x;
    ret[1] = v.y;
    ret[2] = v.z;
    ret[3] = value;
    return ret;
}

Matrix Tools::ULookUp(Vec3f center, Vec3f eye, Vec3f up)
{
    Vec3f k = (eye - center).normalize();
    Vec3f j = up.normalize();
    Vec3f i = cross(j, k).normalize();

    mat<3, 3, float> M;
    M.set_col(0, i); M.set_col(1, j); M.set_col(2, k);

    auto M_inv = M.invert();
    Matrix ret;
    ret.set_col(0, V3f_to_V4f(M_inv.col(0), 0));
    ret.set_col(1, V3f_to_V4f(M_inv.col(1), 0));
    ret.set_col(2, V3f_to_V4f(M_inv.col(2), 0));
    ret.set_col(3, V3f_to_V4f(center * -1, 1));

    return ret;
}
