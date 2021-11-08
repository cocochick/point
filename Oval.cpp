#include "Oval.h"
#include "method.h"
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace Eigen;

Oval::Oval(int xc, int yc, int r_x, int r_y,QPen &pen_value)
    :pen(pen_value), center(xc,yc), radius_x(r_x), radius_y(r_y){}

Oval::Oval(QPoint start, QPoint end,QPen &pen_value):pen(pen_value){
    center = QPoint(start.x(),end.y());
    radius_x = abs(start.x()-end.x());
    radius_y = abs(start.y()-end.y());
}

Oval::Oval(const Oval &o):pen(o.pen),center(o.center),radius_x(o.radius_x),radius_y(o.radius_y){}


void Oval::midpoint(int xCtr, int yCtr, int rx, int ry,QPainter &painter) {
    int x = 0, y = ry;
    int srx = rx*rx;
    int sry = ry*ry;
    int p1 = sry - srx * ry + (srx >> 2);
    Complete(xCtr, yCtr + ry,painter);
    while(sry*x<srx*y){
        if(p1<0){
            Complete(++x + xCtr, y + yCtr, painter);
            p1 = p1 + sry*(2*x + 1);
        }else{
            Complete(++x + xCtr, --y + yCtr, painter);
            p1 = p1 + 2*sry * x - 2 * srx * y + sry;
        }
    }
    int p2 = sry*(x*x+x)+ (sry>>2) + srx*(y-1)*(y-1) -sry*srx;
    while(y!=0){
        if(p2>0){
            Complete(x + xCtr, --y + yCtr, painter);
            p2 = p2 + srx*(1 - 2*y);
        }else{
            Complete(++x + xCtr, --y + yCtr, painter);
            p2 = p2 + 2*sry * x - 2 * srx * y + srx;
        }
    }
}

void Oval::draw(QPainter &painter){
    painter.setPen(pen);
    midpoint(center.x(),center.y(),radius_x,radius_y,painter);
}

void Oval::Complete(int x, int y,QPainter &painter) {
    painter.drawPoint(x,y);
    painter.drawPoint(2*center.x()-x,y);
    painter.drawPoint(x,2*center.y()-y);
    painter.drawPoint(2*center.x()-x,2*center.y()-y);
}

QPoint Oval::getCenter(){
    return center;
}
int Oval::getRadius_x(){
    return radius_x;
}
int Oval::getRadius_y(){
    return radius_y;
}

void Oval::translate(int x, int y){
    center = QPoint(x,y);
}

void Oval::rotate(int x, int y, int dest_x, int dest_y){
    double angle = get_angle(center.x(),center.y(),dest_x,dest_y,x,y);
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
    Matrix<double,3, 1> org{(double)center.x(),(double)center.y(),1};
    Matrix<double,3,3> M = trans_org * rotate_M * trans_later;
    Matrix<double,3,1> res = M * org;
    center.setX(int(res(0,0)+0.5));
    center.setY(int(res(1,0)+0.5));
}

void Oval::scale(double value){
    this->radius_x = value * this->radius_x;
    this->radius_y = value * this->radius_y;
}

void Oval::clear(QPainter &painter){
    QPen tmp = pen;
    pen.setColor(Qt::white);
    painter.setPen(tmp);
    draw(painter);
    pen = tmp;
}
