#ifndef GL
#define GL

#include"matrix&vector_math.h"
#include"tgaimage.h" 
extern Matrix model_view;
extern Matrix view;
extern Matrix projection;

void view(int x,int y,int w,int h);
void projection(float coeff=0.0f);
void loolat(Vec3f eye,Vec3f center,Vec3f up);

class ishader{
    public:
    virtual ~ishader();
    virtual Vec4f vertex(int iface,int nthvert)=0;
    virtual bool fragment(Vec3f bar,TGAColor &color)=0;
};

void triangle(Vec4f *pts,ishader &shader,TGAimage &image,TGAimage &zbuffer);

#endif