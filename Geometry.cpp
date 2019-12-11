
#include "Geometry.h"
#include <algorithm>
//TODO: POLYGON
Polygon::Polygon(std::vector<Point> vert, std::vector<Face> faces, std::vector<float>specs) {
    this -> count = vert.size();
    this -> edgeCount = edges.size();
    this -> faceCount = faces.size();
    
    for(int i = 0; i < count; i++){
        //this->vertices.push_back(std::vector<vec3f>{vert[i].x(), vert[i].y(), vert[i].z()});
        this->vertices.push_back(vert[i]);
    }
    for(int k=0;k<faces.size();k++){
        //this->faces.push_back(std::vector<Face>{faces[k].f0, faces[k].f1, faces[k].f2});
        this->faces.push_back(faces[k]);
    }
    for(int k=0;k<faces.size();k++){
        this->specs.push_back(specs[k]);
    }
    this->updateNormalforfaces();
    this->updateSpectwithfaces();
    //this -> updateCentroid();
}

void Polygon::printPolygon() {
    std::cout<<"numVertices: "<<this->count<<std::endl;
    for(int i = 0; i < this->count; i++) {
        std::cout<<"x: "<< this->vertices[i].point.x()<<" y: "<<this->vertices[i].point.y()<<" z: "<<this->vertices[i].point.y()<<std::endl;
    }
    std::cout<<std::endl;
}
void Polygon::updateNormalforfaces(){
    for(int i = 0;i < faces.size()-1; i++) {
        Face &f = this->faces[i];
        vec3f a = this->vertices[f.f1].point - this->vertices[f.f0].point;
        vec3f b = this->vertices[f.f2].point - this->vertices[f.f0].point;
        vec3f n = (a.cross(b)).normalized();
        f.normal = n;
    }
}

void Polygon::updateCentroid() {
    float xtotal = 0, ytotal = 0, ztotal=0;
    for(int i = 0; i < this->count; i++){
        xtotal += this->vertices[i].point.x();
        ytotal += this->vertices[i].point.y();
        ztotal += this->vertices[i].point.y();
    }
    this->position = {xtotal/(float)(this->count), ytotal/(float)(this->count),  ztotal/(float)(this->count)};
}
void Polygon::updateSpectwithfaces(){
    if(faces.size()==specs.size()){
        
        for(int i=0;i<faces.size();i++){
            faces[i].spectNum = this->specs[i];
        }
    }else{
        std::cout<<"Facenum didnt match spectNum!!!"<<std::endl;
    }
}
float Polygon:: getZMin(Face face){
    Point a = this->vertices[face.f0];
    Point b = this->vertices[face.f1];
    Point c = this->vertices[face.f2];
    return fmin(a.point.z(),fmin(b.point.z(),c.point.z()));
}
float Polygon:: getXMin(Face face){
    Point a = this->vertices[face.f0];
    Point b = this->vertices[face.f1];
    Point c = this->vertices[face.f2];
    return fmin(a.point.x(),fmin(b.point.x(),c.point.x()));
}
float Polygon:: getYMin(Face face){
    Point a = this->vertices[face.f0];
    Point b = this->vertices[face.f1];
    Point c = this->vertices[face.f2];
    return fmin(a.point.y(),fmin(b.point.y(),c.point.y()));
}


std::vector<int> Polygon::sortZ() {
    int i, index;
    float min;
    std::vector<std::pair<float, int>> pairList;
    pairList.clear();
    std::vector<int> res;
    res.clear();

    for (index = 0; index < faces.size(); index++) {
        min = getZMin(faces[index]);
        pairList.push_back(std::make_pair(min, index));
    }
    sort(pairList.begin(), pairList.end(), std::greater<std::pair<float,int>>());
//    sort(pairList.begin(), pairList.end());
    for (i = 0 ; i < faces.size(); i++){
        res.push_back(pairList[i].second);
    }
    return res;
}
std::vector<int> Polygon::sortY() {
    int i, index;
    float min;
     std::vector<std::pair<float, int>> pairList;
       pairList.clear();
       std::vector<int> res;
       res.clear();
    for (index = 0; index < faces.size(); index++) {
        min = getYMin(faces[index]);
        pairList.push_back(std::make_pair(min, index));
    }
    sort(pairList.begin(), pairList.end(),std::greater<std::pair<float,int>>());
//    sort(pairList.begin(), pairList.end());
    for (i = 0 ; i < faces.size(); i++){
        res.push_back(pairList[i].second);
    }
    return res;
}
std::vector<int> Polygon::sortX() {
    int i, index;
    float min;
    std::vector<std::pair<float, int>> pairList;
    pairList.clear();
    std::vector<int> res;
    res.clear();

    for (index = 0; index < faces.size(); index++) {
        min = getXMin(faces[index]);
        pairList.push_back(std::make_pair(min, index));
    }
    sort(pairList.begin(), pairList.end(),std::greater<std::pair<float,int>>());
//    sort(pairList.begin(), pairList.end());
    for (i = 0 ; i < faces.size(); i++){
        res.push_back(pairList[i].second);
//        std::cout<<pairList[i].second<<std::endl;
    }
    return res;
}


