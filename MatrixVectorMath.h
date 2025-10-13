#pragma once
#include<cmath>
#include<iostream>
#include<cassert>
#include<vector>

template<size_t row,size_t col,typename T>class mat;

template<size_t dim,typename T> struct vec{
    vec(){ for(size_t i=dim;i--;data_[i]=T());}
          T& operator [](const size_t i)      {assert(i<dim);return data_[i];}
    const T& operator [](const size_t i)const {assert(i<dim);return data_[i];}
private:
    T data_[dim];
}; 

template<typename T> struct vec<2,T>{
    vec():x(T()),y(T()){}
    vec(T X,T Y):x(X),y(Y){}
    template <typename U> vec(const vec<2,U> &v);
          T& operator [](const size_t i)      {assert(i<2);return  i<=0?x:y;}
    const T& operator [](const size_t i)const {assert(i<2);return i<=0?x:y;}
    T x,y;
};

template<typename T> struct vec<3,T> {
    vec() :x(T()),y(T()),z(T()) {}
    vec(T X,T Y,T Z):x(X),y(Y),z(Z) {}
    template <typename U> vec(const vec<3,U> &v);
          T& operator [](const size_t i)      {assert(i<3);return i<=0?x:(i==1?y:z);}
    const T& operator [](const size_t i)const {assert(i<3);return i<=0?x:(i==1?y:z);}
    float norm() {return std::sqrt(x*x+y*y+z*z); }
    vec<3,T> & normalize(T l=1) {*this=(*this)*(l/norm());return *this;}

    T x,y,z;
};

template<typename T>
struct vec<4,T> {
    vec() : x(T()), y(T()), z(T()), w(T()) {}
    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    template<typename U> vec(const vec<4,U>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    T& operator[](const size_t i)       { assert(i<4); return i==0?x:(i==1?y:(i==2?z:w)); }
    const T& operator[](const size_t i) const { assert(i<4); return i==0?x:(i==1?y:(i==2?z:w)); }

    float norm() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    vec<4,T>& normalize(T l=1) { float n=norm(); if(n>1e-8) *this = (*this)*(l/n); return *this; }

    T x, y, z, w;
};
template<size_t dim,typename T> T operator*(const vec<dim,T>& a,const vec<dim,T>& b){
    T res=T();
    for(size_t i=dim;i--;res+=a[i]*b[i]);
    return res;
}

template<size_t dim,typename T> vec<dim,T> operator+(vec<dim,T> a,vec<dim,T> b){
    for(auto i=dim;i--;a[i]+=b[i]);
    return a;
}

template<size_t dim,typename T> vec<dim,T> operator-(vec<dim,T> a,vec<dim,T> b){
    for(auto i=dim;i--;a[i]-=b[i]);
    return a;
}

template<size_t dim,typename T,typename U> vec<dim,T> operator*(vec<dim,T> a,const U& b){
    for(auto i=dim;i--;a[i]*=b);
    return a;
}

template<size_t dim,typename T,typename U> vec<dim,T> operator/(vec<dim,T> a,const U& b){
    for(auto i=dim;i--;a[i]/=b);
    return a;
}

template<size_t len,size_t dim,typename T> vec<len,T> embed(const vec<dim,T> &v,T null=1){
    vec<len,T> res;
    for(size_t i=len;i--;res[i]=(i<dim?v[i]:null));
    return res;
}

template<size_t len,size_t dim,typename T> vec<len,T> proj(const vec<dim,T> &v){
    vec<len,T> ret;
    for(size_t i=len;i--;ret[i]=v[i]);
    return ret;
}

template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t dim,typename T> std::ostream& operator<<(std::ostream& out,vec<dim,T>& v){
    for(unsigned int i=0;i<dim;i++){
        out<<v[i]<<" ";
    }
    return out;
}

template<size_t dim,typename T> struct dt{
    static T det(const mat<dim,dim,T>& src){
        T ret=0;
        for(size_t i=dim;i--;ret+=src[0][i]*src.cofactor(0,i));
        return ret;
    }
};//计算行列式

template<typename T> struct dt<1,T>{
    static T det(const mat<1,1,T>& src){
        return src[0][0];
    }
};

template<size_t row,size_t col,typename T> class mat{
    vec<col,T> rows[row];
    public:
        mat(){}

        vec<col,T>& operator[](const size_t idx){
            assert(idx<row);
            return rows[idx];
        }

        const vec<col,T>& operator[](const size_t idx)const{
            assert(idx<row);
            return rows[idx];   
        }
        vec<row,T> Col(const size_t idx)const{
            assert(idx<col);
            vec<row,T> res;
            for(size_t i=row;i--;res[i]=rows[i][idx]);
            return res;
        }

        void set_col(size_t idx,vec<row,T> v){
            assert(idx<col);
            for(size_t i=row;i--;rows[i][idx]=v[i]);
        }
        static mat<row,col,T> identity(){
            mat<row,col,T> res;
            for(size_t i=row;i--;){
                for(size_t j=col;j--;){
                    res[i][j]=(i==j);
                }
            }
            return res;
        }
        T det()const {
            return dt<col,T>::det(*this);
        }
        mat<row-1,col-1,T> get_minor(size_t row_,size_t col_)const{
            mat<row-1,col-1,T> res;
            for(size_t i=row-1;i--;){
                for(size_t j=col-1;j--;){
                    res[i][j]=rows[i<row_?i:i+1][j<col?j:j+1];
                }
            }
            return res;
        }

        T cofactor(size_t row_,size_t col_)const{
            return get_minor(row_,col_).det()*((row_+col_)%2?-1:1);
        }

        mat<row,col,T> adjugate(){
            mat<row,col,T> res;
            for(size_t i=row;i--;){
                for(size_t j=col;j--;){
                    res[i][j]=cofactor(i,j);
                }
            }
            return res;
        }

        mat<row,col,T> invert_transpose() {
            mat<row,col,T> ret = adjugate();
            T tmp = ret[0]*rows[0];
            return ret/tmp;
        }
};

template<size_t DimRows,size_t DimCols,typename T> vec<DimRows,T> operator*(const mat<DimRows,DimCols,T>& lhs, const vec<DimCols,T>& rhs) {
    vec<DimRows,T> ret;
    for (size_t i=DimRows; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<size_t R1,size_t C1,size_t C2,typename T>mat<R1,C2,T> operator*(const mat<R1,C1,T>& lhs, const mat<C1,C2,T>& rhs) {
    mat<R1,C2,T> result;
    for (size_t i=R1; i--; )
        for (size_t j=C2; j--; result[i][j]=lhs[i]*rhs.Col(j));
    return result;
}

template<size_t DimRows,size_t DimCols,typename T>mat<DimCols,DimRows,T> operator/(mat<DimRows,DimCols,T> lhs, const T& rhs) {
    for (size_t i=DimRows; i--; lhs[i]=lhs[i]/rhs);
    return lhs;
}

template <size_t DimRows,size_t DimCols,typename T> std::ostream& operator<<(std::ostream& out, mat<DimRows,DimCols,T>& m) {
    for (size_t i=0; i<DimRows; i++) out << m[i] << std::endl;
    return out;
}


typedef vec<2,  float> Vec2f;
typedef vec<2,  int>   Vec2i;
typedef vec<3,  float> Vec3f;
typedef vec<3,  int>   Vec3i;
typedef vec<4,  float> Vec4f;
typedef mat<4,4,float> Matrix;