#ifndef GL
#define GL

#include"matrix_vector_math.h"
#include"tgaimage.h" 
extern Matrix modelview;
extern Matrix View;
extern Matrix projection_;

void view(int x,int y,int w,int h);
void projection(float coeff=0.0f);
void lookat(Vec3f eye,Vec3f center,Vec3f up);

class ishader{
    public:
    virtual ~ishader();
    virtual Vec4f vertex(int iface,int nthvert)=0;
    virtual bool fragment(Vec3f bar,TGAColor &color)=0;
};

void triangle(Vec4f *pts,ishader &shader,TGAimage &image,TGAimage &zbuffer);

#endif