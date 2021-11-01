#include "Polygen.h"
#include <iostream>
#include "method.h"
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace Eigen;

Polygen::Polygen(std::vector<QPoint> p):point(p) {
    for (auto iter = p.begin(); iter != p.end() - 1; iter++) {
        Line line(*iter, *(iter + 1));
        vertex.push_back(line);
    }
    Line line(*p.begin(), *(p.end() - 1));
    vertex.push_back(line);
}



void Polygen::draw(QPainter &painter){
    for (auto &line : vertex) {
        line.drawByBresenham(painter);
    }
}



void Polygen::setVertex(){   
    vertex.clear();
    for (auto iter = point.begin(); iter != point.end() - 1; iter++) {
        Line line(*iter, *(iter + 1));
        vertex.push_back(line);
    }
    Line line(*point.begin(), *(point.end() - 1));
    vertex.push_back(line);
}

std::vector<QPoint> Polygen::getPoint() const{
    return this->point;
}

void Polygen::translate(int x, int y){
    int trans_x = x - point[0].x();
    int trans_y = y - point[0].y();

    for(auto beg = point.begin();beg != point.end();++beg){
        beg->setX(beg->x() + trans_x);
        beg->setY(beg->y() + trans_y);
    }
    setVertex();
}


void Polygen::rotate(int x, int y, int dest_x, int dest_y){
    double angle = get_angle(point[0].x(),point[0].y(),dest_x,dest_y,x,y);
    double tx = x; double ty = y;
    Matrix<double,3,3>  trans_org{
        {1, 0, tx},
        {0 ,1 ,ty},
        {0 ,0 ,1}};
    Matrix<double,3,3> rotate_M{
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0, 0, 1}};
    Matrix<double,3,3> trans_later{
        {1, 0, -tx},
        {0 ,1 ,-ty},
        {0 ,0 ,1}};
    Matrix<double,3,3> M = trans_org * rotate_M * trans_later;
    for( auto &p: point){
        Matrix<double,3, 1> org{(double)p.x(),(double)p.y(),1};
        Matrix<double,3,1> res = M * org;
        p.setX(int(res(0,0)+0.5));
        p.setY(int(res(1,0)+0.5));
    }
    setVertex();
}
