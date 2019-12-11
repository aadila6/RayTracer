#include "vector.h"
#include "matrix.h"
#include <vector>
using namespace std;
typedef mmm::vector<2, float> vec2f;
typedef mmm::vector<3, float> vec3f;
typedef mmm::vector<2, int> vec2i;
typedef mmm::vector<3, int> vec3i;

typedef mmm::matrix<3, 3, float> mat3f;
typedef mmm::matrix<4, 4, float> mat4f;

struct Edge
{
    int start, end;
    Edge(int start, int end) {
        this->start=(start);
        this->end=(end);
    }
    
};
struct RGB
{
    float r, g, b;
    RGB(){
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    RGB(float c0, float c1, float c2) {
        this->r = c0;
        this->g = c1;
        this->b = c2;
    }
};

struct Point
{
    vec3f point;
    RGB intensity;
    Point(vec3f inputPoint, RGB intensity){
        this->point = inputPoint;
        this->intensity= intensity;
    }
    Point(vec3f inputPoint){
        this->point = inputPoint;
        RGB temp(0,0,0);
        this->intensity = temp;
    }
};

struct Face
{
    int f0, f1, f2;
    vec3f normal;
    float spectNum;
    Face(int f0, int f1, int f2) : normal()
    {
        this->f0=(f0);
        this->f1=(f1);
        this->f2=(f2);
    }
};

struct PhongModel{
    RGB ambient;
    RGB kd;
    RGB ks;
    float IL;
    vec3f fPoint;
    vec3f pPoint;
    vec3f lPoint;
    vec3f nVector;
    vec3f lVector;
    vec3f rVector;
    vec3f vVector;
    float K;
    float n;
    RGB phongrgb;
    PhongModel(){
        this->ambient = {5,5,5};
        this->kd = {0, 0, 0};
        this->ks = {255,255,255};
        this->IL = 0.2;
        this->pPoint = {0, 0, 0};
        this->lPoint = {1, 1, 0};
        this->K = 20;
        this->n = 1;
        this->lVector = lPoint - pPoint;
        this->vVector = fPoint - pPoint;
    }
    
    PhongModel(Point dot, vec3f facenormal,vec3f frontPoint, vec3f light, float spec, RGB amb){
        this->ambient = amb;
        this->kd = dot.intensity; //
        this->ks = {1,0,0}; //color of light source
        this->IL = 0.8;
        this->fPoint = frontPoint;
        this->pPoint = dot.point;
        this->lPoint = light;
        this->nVector = facenormal;
        this->lVector = (lPoint - pPoint).normalized();
        vec3f ntwo = nVector*2.0;
        float num = ntwo.dot(lVector);
        this->rVector = ((nVector*num) - lVector).normalized();
        this->vVector = (fPoint - pPoint).normalized();
        this->K = 1;
        this->n = spec;
        this->phongrgb = calculatePhong(dot);
    }
    
    RGB calculatePhong(Point dot){
        float rnumber = 0;
        float gnumber = 0;
        float bnumber = 0;
        rnumber = ambient.r + IL*(kd.r*(lVector.dot(nVector)) + pow(ks.r*(rVector.dot(vVector)),n) )/((vVector).norm()+0.5);
        gnumber = ambient.g + IL*(kd.g*(lVector.dot(nVector)) +pow(ks.g*(rVector.dot(vVector)),n) )/((vVector).norm()  + K);
        bnumber = ambient.b + IL*(kd.b*(lVector.dot(nVector)) +pow(ks.b*(rVector.dot(vVector)),n) )/((vVector).norm()  + K);
        if(abs(rnumber) > 1){
            rnumber = 1;
        }
        if(abs(gnumber) > 1){
            gnumber = 1;
        }
        if(abs(bnumber) > 1){
            bnumber = 1;
        }
        RGB value(abs(rnumber),abs(gnumber),abs(bnumber));
        return value;
    }
};

struct Sphere{
    vec3f center;
    float radius;
    Sphere(const vec3f &c, const float r){
        this->center = c;
        this->radius = r;
    }
    bool ray_intersect(vec3f &orig, vec3f &dir, float &t0){
        vec3f o = orig;
        vec3f c = center;
        vec3f l = dir;
        vec3f R = o - c;
        float r = radius;
        float A = -(l.dot(R));
        float radicand = (l.dot(R))*(l.dot(R)) - (R.norm() * R.norm()) + r*r;
        if (radicand < 0.0f)
            return false;
        
        return true;
    }
};
