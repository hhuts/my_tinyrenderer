#ifndef _MODEL
#define _MODEL
#include<vector>
#include<string>
#include"MatrixVectorMath.h"
#include"tgaimage.h"
class model{
    private:
        std::vector<Vec3f> verts_;
        std::vector<std::vector<Vec3i>> faces_;
        std::vector<Vec3f> norms_;
        std::vector<Vec2f> uv_;
        TGAimage diffusemap_;
        TGAimage normalmap_;
        TGAimage specularmap_;
        void load_texture(std::string fileaname,const char *suffix,TGAimage &img);
    public:
        model(const char *filename);
        ~model();
        int nverts();
        int nfaces();
        Vec3f normal(int iface,int nthvert);
        Vec3f normal(Vec2f uv);
        Vec3f vert(int i);
        Vec3f vert(int iface, int nthvert);
        Vec2f uv(int iface, int nthvert);
        TGAColor diffuse(Vec2f uv);
        float specular(Vec2f uv);
        std::vector<int> face(int idx);
};
#endif
