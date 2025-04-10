#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
 
#pragma pack(push,1)
class TGA_HEAD{

    public:

    char idlength;//图像id字符串长度
    char colormaptype;//调色板类型
    char datadypecode;//图像类型码
    short cololrmaporigin;//调色板起始位置
    short colormaplength;//调色板颜色数量
    char colormapdepth;//调色板每个颜色的位数
    short x_origin;//图像左下角x坐标
    short y_origin;//左下角y坐标
    short width;//宽带
    short height;//高度
    short bitsperpixel;//每像素位数
    char imagedescriptor;//图像描述符
};
#pragma pack(pop)

class TGAColor{
    unsigned char bgra[4];
    unsigned char bytespp;

    public:

    TGAColor():bgra(),bytespp(1){
        for(auto& i:bgra){
            i=0;
        }
    }

    TGAColor(unsigned char R,unsigned char G,unsigned char B,unsigned char A=255){
        bgra[0]=B;
        bgra[1]=G;
        bgra[2]=R;
        bgra[3]=A;
    }

    TGAColor(unsigned char v):bgra(),bytespp(1){
        for(auto& i:bgra){
            i=0;
        }
        bgra[0]=v;
    }

    TGAColor(const unsigned char *p,unsigned char bpp):bgra(),bytespp(bpp){
        for(int i=0;i<4;++i){
            bgra[i]=i<(int)bpp?p[i]:0;
        }
    }

    unsigned char &operator[](const int i){return bgra[i];}

    TGAColor operator*(float num)const{
        TGAColor res=*this;
        num=(num<0.f?0.f:(num>1.f?1.f:num));
        for(int i=0;i<4;++i){
            res.bgra[i]=bgra[i]*num;
        }
        return res;
    }
};

class TGAimage{
    protected:
        unsigned char *data;
        int width;
        int height;
        int bytespp;

        bool load_rle_data(std::ifstream &in);
        bool unload_rle_data(std::ostream &out);
    public:
        enum format{
            B=1,RGB=3,RGBA=4
        };

        TGAimage();
        TGAimage(int w,int h,int bytespp);
        TGAimage(const TGAimage &img);
        bool read_tga_file(const char *filename);
        bool write_tga_file(const char *filename,bool rle=true);
        bool horizontally();
        bool vertically();
        bool scale(int w,int h);
        TGAColor get_color(int x,int y);
        bool set_color(int x,int y,TGAColor &c);
        bool set_color(int x,int y,TGAColor const &c);
        ~TGAimage();
        TGAimage & operator= (const TGAimage &image);
        int get_width();
        int get_height();
        int get_bytespp();
        unsigned char *buffer();
        void clear();
};
#endif 