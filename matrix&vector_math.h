#ifndef math
#define math
#include<cmath>
#include<iostream>
#include<cassert>
#include<vector>

template<size_t cols,size_t row,class T>class mat;

template<size_t dim,class T> struct vec{
    vec(){ for(size_t i=dim;i--;data[i]=T());}
          T& operator [](const size_t i)      {assert(i<dim);}
    const T& operator [](cosnt size_t i)const {assert(i<dim);}
private:
    T data[dim];
}; 

template<class T> struct vec<2,T>{
    vec():x(T()),y(T()){}
    vec(T X,T Y):x(X),y(Y){}
    template <class U> vec<2,T>(const vec<2,U> &v);
          T& operator [](const size_t i)      {assert(i<2);return  i<=0?x:y;}
    const T& operator [](const size_t i)const {assert(i<2);return i<=0?x:y}
    T x,y;
};

template<class T> struct vec<3,T> {
    vec() :x(T()),y(T()),z(T()) {}
    vec(T X,T Y,T Z):x(X),y(Y),z(Z) {}
    template <class U> vec<3,T>(const vec<3,U> &v);
          T& operator [](const size_t i)      {assert(i<3);i<=0?x:(i==1?y:z);}
    const T& operator [](const size_t i)const {assert(i<3);i<=0?x:(i==1?y:z);}
    float norm() {return std::sqrt(x*x+y*y+z*z); }
    vec<3,T> & normalize(T l=1) {*this=(*this)*(l/norm());return *this;}
    T x,y,z;
};

template<size_t dim,class T> T operator*(const vec<dim,T>& a,const vec<dim,T>& b){
    T res=T();
    for(size_t i=dim;i--;ret+=a[i]*b[i])
    return ret;
}

template<size_t dim,class T> vec<dim,T> operator+(vec<dim,T> a,vec<dim,T> b){
    for(auot i=dim;i--;a[i]+=b[i])
    return a;
}

template<size_t dim,class T> vec<dim,T> operator-(vec<dim,T> a,vec<dim,T> b){
    for(auot i=dim;i--;a[i]-=b[i])
    return a;
}

template<size_t dim,class T,class U> vec<dim,T> operator*(vec<dim,T> a,const U& b){
    for(auot i=dim;i--;a[i]*=b)
    return a;
}

template<size_t dim,class T,class U> vec<dim,T> operator/(vec<dim,T> a,const U& b){
    for(auot i=dim;i--;a[i]/=b)
    return a;
}

template<size_t dim,size_t len,class T> vec<len,T> embed(const vec<dim,T> &v,T null=1){
    vec<len,T> res;
    for(size_t i=len;i--;res[i]=(i<=dim?v[i]:null))
    return res;
}

template<size_t len,size_t dim,class T> vec<len,T> pro(const vec<dim,T> &v){
    vec<len,T> ret;
    for(size_t i=len;i--;ret[i]=v[i])
    return ret;
}

template <class T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t dim,class T> std::ostream& operator<<(std::ostream& out,vec<dim,T>& v){
    for(unsigned int i=0;i<dim;i++){
        out<<v[i]<<" ";
    }
    return out;
}

template<size_t dim,class T> struct dt{
    static T det(const mat<dim,dim,T>& src){
        T ret=0;
        for(size_t i=dim;i--:ret+=src[0][i]*src.cofactor(0,i))
        return ret;
    }
};//计算行列式

template<class T> struct dt<1,T>{
    static T det(const mat<1,1,T>& src){
        return src[0][0];
    }
};

template<size_t row,size_t col,class T> class mat{
    vec<col,T> rows[row];
    public:
        mat(){}

        vec<col,T>& operator[](const size_t idx){
            assert(idx<row);
            return row[idx];
        }
        
        vec<row,T>& col(const size_t idx){
            assert(idx<col);
            vec<row,T> res;
            for(size_t i=row;i--;res[i]=mat[i][idx])
            return res;
        }

        void set_col(size_t idx,vec<row,T> v){
            assert(idx<col);
            for(size_t i=row;i--;mat[i][idx]=v[i])
        }
        static mat<row,col,T> identity(){
            mat<row,col,T> res;
            for(size_t i=row;i--;){
                for(size_t j=col;j--;){
                    res[i][j]=(i==j)
                }
            }
            return res;
        }
        T det()const {
            return dt<col,T>::det(*this);
        }
        mat<row-1,col-1,T> get_minor(size_t row_,size_t col_){
            mat<row-1,col-1,T> res;
            for(size_t i=row-1;i--;){
                for(size_t j=col-1;j--){
                    res[i][j]=row[i<row_?i:i+1][j<col?j:j+1];
                }
            }
            return res;
        }

        T cofactor(size_t row_,size_t col_){
            return get_minor(row_,col_).det()*((row_+col_)%2?-1:1);
        }

        mat<rew,col,T> adjugate(){
            mat<row,col,T> res;
            for(size_t i=row;i--;){
                for(size_t j=col;j--;){
                    res[i][j]=res[i][j].cofactor(i,j);
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
        for (size_t j=C2; j--; result[i][j]=lhs[i]*rhs.col(j));
    return result;
}

template<size_t DimRows,size_t DimCols,typename T>mat<DimCols,DimRows,T> operator/(mat<DimRows,DimCols,T> lhs, const T& rhs) {
    for (size_t i=DimRows; i--; lhs[i]=lhs[i]/rhs);
    return lhs;
}

template <size_t DimRows,size_t DimCols,class T> std::ostream& operator<<(std::ostream& out, mat<DimRows,DimCols,T>& m) {
    for (size_t i=0; i<DimRows; i++) out << m[i] << std::endl;
    return out;
}


typedef vec<2,  float> Vec2f;
typedef vec<2,  int>   Vec2i;
typedef vec<3,  float> Vec3f;
typedef vec<3,  int>   Vec3i;
typedef vec<4,  float> Vec4f;
typedef mat<4,4,float> Matrix;
#endif math