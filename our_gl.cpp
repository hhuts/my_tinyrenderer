#include <cmath>
#include <limits>
#include <cstdlib>
#include "our_gl.h"
#include <algorithm>
Matrix modelview;
Matrix View;
Matrix projection_;

ishader::~ishader(){}

void view(int x, int y, int w, int h){
    View = Matrix::identity();
    View[0][3] = x+w/2.f;
    View[1][3] = y+h/2.f;
    View[2][3] = 255.f/2.f;
    View[0][0] = w/2.f;
    View[1][1] = h/2.f;
    View[2][2] = 255.f/2.f;
}

void projection(float coeff) {
    projection_ = Matrix::identity();
    projection_[3][2] = coeff;
}

void lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye-center).normalize();
    Vec3f x = cross(up,z).normalize();
    Vec3f y = cross(z,x).normalize();
    modelview = Matrix::identity();
    Matrix translaition = Matrix::identity();
	Matrix rotation     = Matrix::identity();
	for (int i = 0; i < 3; i++) {
        translaition[i][3] = -center[i];
	}
    for (int i=0; i<3; i++) {
        rotation[0][i] = x[i];
        rotation[1][i] = y[i];
        rotation[2][i] = z[i];
    }
    modelview = rotation* translaition;
}

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec3f s[3];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) 
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1);
}

// Rasterize a triangle defined by three homogeneous points.  `image`
// stores the final color while `zbuffer` keeps track of depth values so
// we don't draw fragments hidden behind others.  The order of the image
// and z-buffer parameters previously differed between the declaration in
// the header and the implementation here, causing them to be swapped when
// calling the function.  As a consequence the depth buffer was written to
// the color image (resulting in the uniform blue output reported by the
// user) and the actual colors were written into the z-buffer.
//
// To fix this we align the parameter order with the declaration in
// `our_gl.h` so that the third argument is the color image and the fourth
// is the z-buffer.  Additionally we correct the computation of the
// interpolated depth value to use the z-component of each vertex instead
// of the first vertex for all components.
void triangle(Vec4f *pts, ishader &shader, TGAimage &image, TGAimage &zbuffer) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            //这里pts除以了最后一个分量，实现了透视中的缩放，所以作为边界框
            bboxmin[j] = std::min(bboxmin[j], pts[i][j]/pts[i][3]);
            bboxmax[j] = std::max(bboxmax[j], pts[i][j]/pts[i][3]);
        }
    }

    Vec2i p;
    TGAColor color;
    for(p.x=bboxmin.x;p.x<=bboxmax.x;p.x++){
        for(p.y=bboxmin.y;p.y<=bboxmax.y;p.y++){
            Vec3f c = barycentric(pro<2>(pts[0] / pts[0][3]),
                                 pro<2>(pts[1] / pts[1][3]),
                                 pro<2>(pts[2] / pts[2][3]),
                                 pro<2>(p));

            float z_p = (pts[0][2] / pts[0][3]) * c.x +
                        (pts[1][2] / pts[1][3]) * c.y +
                        (pts[2][2] / pts[2][3]) * c.z;
            int frag_depth = std::max(0, std::min(255, int(z_p + .5f)));

            if (c.x < 0 || c.y < 0 || c.z < 0 ||
                zbuffer.get_color(p.x, p.y)[0] > frag_depth)
                continue;

            bool discard = shader.fragment(c, color);

            if (!discard) {
                zbuffer.set_color(p.x, p.y, TGAColor(frag_depth));
                image.set_color(p.x, p.y, color);
            }
        }
    }
}