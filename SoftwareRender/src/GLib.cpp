#include "GLib.h"

Vec3f BaryCentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f AB = B - A, AC = C - A, PA = A - P;
    Vec3f vec1{ AB.x, AC.x ,PA.x }, vec2{ AB.y, AC.y, PA.y };
    Vec3f uv = cross(vec1, vec2);
    if (std::abs(uv[2]) > 1e-2)
        return { 1.f - (uv.x + uv.y) / uv.z, uv.x / uv.z, uv.y / uv.z };
    return { -1, 1, 1 };
}

Vec3f V4f_to_V3f(Vec4f v) {
    return { v[0] / v[3], v[1] / v[3],v[2] / v[3] };
}
Vec4f V3f_to_V4f(Vec3f v, float value)
{
    Vec4f ret;
    ret[0] = v.x;
    ret[1] = v.y;
    ret[2] = v.z;
    ret[3] = value;
    return ret;
}


void Context::glSetVertArrayCoord()
{
    assert(m_VertexArrayCoord.size() == 0 );
    if (!m_VertexBuffer.size()) return;

    for (const auto& elem: m_VertexBuffer) {
        m_VertexArrayCoord.push_back(elem.m_VertexCoord);
    }
    
}

void Context::glsetVertArrayTexCoord()
{
    assert(m_VertexArrayTexCoord.size() == 0);
    if (!m_VertexBuffer.size()) return;

    for (const auto& elem : m_VertexBuffer) {
        m_VertexArrayTexCoord.push_back(elem.m_TextureCoord);
    }
}

void Context::glSetVertArrayNormal()
{
    assert(m_VertexArrayNormal.size() == 0);
    if (!m_VertexBuffer.size()) return;

    for (const auto& elem : m_VertexBuffer) {
        m_VertexArrayNormal.push_back(elem.m_Normal);
    }
}





