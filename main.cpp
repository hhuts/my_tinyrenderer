#include <vector>
#include <iostream>
#include <algorithm>
#include "tgaimage.h"
#include "model.h"
#include "MatrixVectorMath.h"
#include "our_gl.h"
#include <chrono>
#include <iomanip>
using namespace std::chrono;
auto start = high_resolution_clock::now();

model *model_=NULL;
const int width=800;
const int height=800;

Vec3f light_dir(0,1,1);
Vec3f eye(1,.5f,1.5f);
Vec3f center(0,0,0);
Vec3f up(0,1,0);

struct PhongShader : public ishader {
    mat<2, 3, float> varying_uv;  // same as above
    mat<4, 4, float> uniform_M = Projection * ModelView;
    mat<4, 4, float> uniform_MIT = ModelView.invert_transpose();
    virtual Vec4f vertex(int iface, int nthvert) {
        varying_uv.set_col(nthvert, model_->uv(iface, nthvert));
        Vec4f gl_Vertex = embed<4>(model_->vert(iface, nthvert)); // read the vertex from .obj file
        return Viewport * Projection * ModelView * gl_Vertex; // transform it to screen coordinates
    }
    virtual bool fragment(Vec3f bar, TGAColor& color) {
        Vec2f uv = varying_uv * bar;
        Vec3f n = proj<3>(uniform_MIT * embed<4>(model_->normal(uv))).normalize();
        Vec3f l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();
        Vec3f r = (n * (n * l * 2.f) - l).normalize();   // reflected light
        
        float spec = pow(std::max(r.z, 0.0f), model_->specular(uv));
        float diff = std::max(0.f, n * l);
        TGAColor c = model_->diffuse(uv);
        color = c;
        for (int i = 0; i < 3; i++) color[i] = std::min<float>(5 + c[i] * (diff + .6 * spec), 255);
        return false;
    }
};

int main(int argc,char **argv)
{
    if(2==argc){
        model_=new model(argv[1]);
    }else{
        model_=new model("D:\\my_tinyrenderer\\obj\\s.obj");
    }

    lookat(eye,center,up);
    projection(-1.f/(eye-center).norm());
    view(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    light_dir.normalize();
    TGAimage zbuffer(width,height,TGAimage::GRAYSCALE);
    TGAimage image(width,height,TGAimage::RGB);
    PhongShader shader;
    int total = model_->nfaces();
    for (int i=0; i<model_->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, image, zbuffer);
        std::cerr<<i<<" ";
    }
    image.filp_vertically();
    zbuffer.filp_vertically();
    image.write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");
    delete model_;
    return 0;
}
