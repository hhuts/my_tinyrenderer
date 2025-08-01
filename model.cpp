#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

model::model(const char *filename):verts_(),faces(),norms(),uv_(),diffusemap_(),normalmap_(),specularmap_() {
    std::ifstream in;
    in.open(filename,std::ifstream::in);
    if(in.fail()) return;
    std::string line;
    while(!in.eof()){
        std::getline(in,line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0,2,"v ")){
            iss>>trash;
            Vec3f v;
            for(int i=0;i<3;i++){
                iss>>v[i];
            }
            verts_.push_back(v);
        }else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            norms.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            uv_.push_back(uv);
        }else if(!line.compare(0,2,"f ")){
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss>>trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces.size() << " vt# " << uv_.size() << " vn# " << norms.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm.tga",      normalmap_);
    load_texture(filename, "_spec.tga",    specularmap_);
}

model::~model(){}

int model::nverts(){
    return (int)verts_.size();
}

int model::nfaces(){
    return (int)faces.size();
}

std::vector<int> model::face(int idx){
    std::vector<int> res;
    for(int i=0;i<(int)faces[idx].size();++i){
        res.push_back(faces[idx][i][0]);
    }
    return res;
}

Vec3f model::vert(int i){
    return verts_[i];
}

Vec3f model::vert(int iface,int nthvert){
    return verts_[faces[iface][nthvert][0]];
}

void model::load_texture(std::string filename,const char *suffix,TGAimage &img){
    std::string texfile(filename);
    size_t dot=texfile.find_last_of(".");
    if(dot!=std::string::npos){
        texfile=texfile.substr(0,dot)=std::string(suffix);
        std::cerr<<"texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.vertically();
    }
}

TGAColor model::diffuse(Vec2f uvf){
    Vec2i uv(uvf[0]*diffusemap_.get_width(),uvf[1]*diffusemap_.get_height());
    return  diffusemap_.get_color(uv[0],uv[1]);
}

Vec3f model::normal(Vec2f uvf){
    Vec2i uv(uvf[0]*normalmap_.get_width(),uvf[1]*normalmap_.get_height());
    TGAColor c=normalmap_.get_color(uv[0],uv[1]);
    Vec3f res;
    for(int i=0;i<3;++i)
        res[2-i]=(float)c[i]/255.f*2.f-1.f;
    return res;
}

Vec2f model::uv(int iface,int nthvert){
    return uv_[faces[iface][nthvert][1]];
}

float model::specular(Vec2f uvf){
    Vec2i uv(uvf[0]*specularmap_.get_width(), uvf[1]*specularmap_.get_height());
    return specularmap_.get_color(uv[0],uv[1])[0]/1.f;
}

Vec3f model::normal(int iface, int nthvert) {
    int idx = faces[iface][nthvert][2];
    return norms[idx].normalize();
}
