#include <vector>
#include <iostream>
#include <algorithm>
#include "tgaimage.h"
#include "model.h"
#include "matrix_vector_math.h"
#include "our_gl.h"

model *model_=NULL;
const int width=800;
const int height=800;

Vec3f light_dir(0,1,1);
Vec3f eye(1,.5f,1.5f);
Vec3f center(0,0,0);
Vec3f up(0,1,0);

struct PhongShader : public ishader {
    mat<2, 3, float> varying_uv;  // same as above
    mat<4, 4, float> uniform_M = projection_ * modelview;
    mat<4, 4, float> uniform_MIT = modelview.invert_transpose();
    virtual Vec4f vertex(int iface, int nthvert) {
        varying_uv.set_col(nthvert, model_->uv(iface, nthvert));
        Vec4f gl_Vertex = embed<3,4,float>(model_->vert(iface, nthvert)); // read the vertex from .obj file
        return View * projection_ * modelview * gl_Vertex; // transform it to screen coordinates
    }
    virtual bool fragment(Vec3f bar, TGAColor& color) {
        Vec2f uv = varying_uv * bar;
        Vec3f n = pro<3>(uniform_MIT * embed<3,4,float>(model_->normal(uv))).normalize();
        Vec3f l = pro<3>(uniform_M * embed<3,4,float>(light_dir)).normalize();
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
        model_=new model("obj/cube.obj");
    }

    lookat(eye,center,up);
    projection(-1.f/(eye-center).norm());
    view(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    light_dir.normalize();
    TGAimage zbuffer(width,height,TGAimage::B);
    TGAimage image(width,height,TGAimage::RGB);
    PhongShader shader;
    for (int i=0; i<model_->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, image, zbuffer);
    }
    image.vertically();
    zbuffer.vertically();
    image.write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    delete model_;
    printf("ok");
    return 0;
}
