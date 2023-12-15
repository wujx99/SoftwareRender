#include "tgaimage.h"

#include "Core.h"
#include "Log.h"
#include "model.h"
#include "geometry.h"
#include "Tools.h"
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
#include <map>


std::pair<std::vector<BufferElemt>, std::vector<TriIndex>> ParseModel(std::shared_ptr<Model>& model)
{
    std::map<int,BufferElemt> bufferMap;
    std::vector<BufferElemt> bufferElems;
    std::vector<TriIndex> triIndexs;

    int nfaces = model->nfaces();
    BufferElemt elem; TriIndex triIndex;
    for (int i = 0; i < nfaces; i++)
    {
        FaceIndex& indexs = model->face(i);
        triIndex = { indexs[0].index,indexs[1].index, indexs[2].index };
        triIndexs.push_back(triIndex);
        for (int j = 0; j < indexs.size(); j++)
        {
            
            elem.m_VertexCoord = model->vert(indexs[j].index);
            elem.m_TextureCoord = model->texCoord(indexs[j].texCoordIndex);
            elem.m_Normal = model->nomal(indexs[j].nomalIndex);
            bufferMap[indexs[j].index] = elem;
        }
    }
    for (auto& mapElem : bufferMap)
    {
        bufferElems.push_back(mapElem.second);
    }
    return{ bufferElems, triIndexs };
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

   
    {
        class GouraudShader :public Shader
        {
        public:
            GouraudShader(Context context) 
            :m_Context(context)
            {};
           

        
            virtual Vec3f Vertex(int triIndex, int vertIndex) override 
            {
                m_TriVertIntensity[vertIndex] = m_Context.Normal(m_Context.TriangleIndex(triIndex)[vertIndex]) *  m_LightDir;
                Vec3f gl_Vertex = m_Context.Vertex(m_Context.TriangleIndex(triIndex)[vertIndex]);
                return V4f_to_V3f( m_ViewPort * m_Proj * m_View * m_Model * embed<4>(gl_Vertex));
            }
            virtual bool Fragment(Vec3f baryCoord, TGAColor& color) override
            {
                float intensity = m_TriVertIntensity * baryCoord;
                color = TGAColor(255, 255, 255) * intensity; 
                return false;
            }
            virtual void Triangle(int triIndex, TGAImage& image, TGAImage& zBuffer)  override
            {
                const int width = image.get_width(), height = image.get_height();
                std::array<Vec3f, 3> pts = { Vertex(triIndex, 0), Vertex(triIndex, 1), Vertex(triIndex, 2) };
                
                 
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
                        auto barycoord = Tools::BaryCentric(proj<2>(pts[0]), proj<2>(pts[1]), proj<2>(pts[2]), Vec2f(float(p.x),float(p.y)));//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////fromtheisiisisisisid
                        for (int i = 0; i < 3; i++)
                        {
                            z += barycoord[i] * pts[i].z;
                        }
                        int frag_depth = std::max(0, std::min(255, int(z + .5)));
                        if (barycoord.x < 0 || barycoord.y < 0 || barycoord.z < 0 || zBuffer.get(p.x, p.y)[0]>frag_depth) continue;
                        bool discard = Fragment(barycoord, color);
                        if (!discard) {
                            zBuffer.set(p.x, p.y, TGAColor(frag_depth));
                            image.set(p.x, p.y, color);
                        }

                    }
                }
            }
            virtual DrawOutput glDraw(int width, int height)  override
            {
                TGAImage retImage(width, height, TGAImage::RGB);
                TGAImage retZBuffer(width, height, TGAImage::GRAYSCALE);
                
                int size = m_Context.IndexBuffer().size();
                for (int i = 0; i < size; i++ ) {
                    Triangle(i, retImage, retZBuffer);
                }
                return { retImage, retZBuffer };
            }
        private:
            // cpu side data
            Context m_Context;

            
        private:
            // for shaders states，
            Vec3f m_TriVertIntensity{};
        public:
            // 相当于是glsl的uniform
            Vec3f m_LightDir{};
            Matrix m_Model{ Matrix::identity() };
            Matrix m_View{ Matrix::identity() };
            Matrix m_Proj{ Matrix::identity() };
            Matrix m_ViewPort{ Matrix::identity() };
        };

        

        Context myContext;
        auto [vertexBuffer, indexBuffer] = ParseModel(model);
        std::cout << "vertexbuffer size: " <<  vertexBuffer.size() << std::endl;
        std::cout << "index buffer size: " << indexBuffer.size() << std::endl;

        

        myContext.glBindVertexBuffer(vertexBuffer);
        myContext.glSetVertArrayCoord();
        myContext.glsetVertArrayTexCoord();
        myContext.glSetVertArrayNormal();
        myContext.glBindIndexBuffer(indexBuffer);
       
        GouraudShader myShader(myContext);

        myShader.m_View = Tools::ULookUp(center, eye, Vec3f(0, 1, 0));
        myShader.m_ViewPort = Tools::ViewPort(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth); //depthh must set to 255;
        Matrix projection = Matrix::identity();
        projection[3][2] = -1.f / (eye - center).norm();
        myShader.m_Proj = projection;
        myShader.m_LightDir =  light_dir;

        auto retimage = myShader.glDraw(width, height);

        retimage.drawedImage.flip_vertically(); // to place the origin in the bottom left corner of the image
        retimage.zbuffer.flip_vertically();
        retimage.drawedImage.write_tga_file("outputs/gshader.tga");
        retimage.zbuffer.write_tga_file("outputs/gshaderzbuffer.tga");

    }
    return 0;
}
