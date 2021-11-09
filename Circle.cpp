
#include "Circle.h"
#include "method.h"
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace Eigen;

Circle::Circle(int x, int y, int r,QPen &pen_value):
    center(x,y), radius(r){pen = pen_value;}

Circle::Circle(const QPoint &cen, int r,QPen &pen_value):
    center(cen),radius(r){pen = pen_value;}

Circle::Circle(const Circle& c){
    pen = c.getpen();
    center = c.getCenter();
    radius = c.getRadius();
    pen = c.pen;
}

void Circle::midpoint(int xCtr, int yCtr,QPainter &painter) {
    int x = 0, y = radius;
    float p = 5.0f/4.0f - static_cast<float>(radius);
    Complete(xCtr, yCtr + radius,painter);
    while(x < y){
        if(p < 0){
            int tmp_x = ++x;
            Complete(tmp_x + xCtr, y + yCtr,painter);
            Complete(y + xCtr, tmp_x + yCtr,painter);
            p = p + 2*x +1;
        }else{
            int tmp_x = ++x, tmp_y = --y;
            Complete(tmp_x + xCtr, tmp_y + yCtr,painter);
            Complete(tmp_y + xCtr, tmp_x + yCtr,painter);
            p = p + 2*x +1 - 2*y;
        }
    }
    painter.drawPoint(center.x() - radius, center.y());
    painter.drawPoint(center.x() + radius, center.y());

}

void Circle::draw(QPainter &painter)  {
    painter.setPen(pen);
    midpoint(center.x(),center.y(),painter);
}

void Circle::Complete(int x, int y,QPainter &painter)  {
    painter.drawPoint(x,y);
    painter.drawPoint(2*center.x()-x,y);
    painter.drawPoint(x,2*center.y()-y);
    painter.drawPoint(2*center.x()-x,2*center.y()-y);
}

void Circle::translate(int x, int y){
   // clear();
   //center = QPoint(x,y);
    center.setX(center.x() + x);
    center.setY(center.y() + y);
}

void Circle::clear(QPainter &painter){
    QPen tmp = pen;
    pen.setColor(Qt::white);
    painter.setPen(tmp);
    draw(painter);
    pen = tmp;
}

QPoint Circle::Center() const{
    return this->center;
}

void  Circle::rotate(int x, int y, int dest_x, int dest_y){
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

void  Circle::scale(double value){
    this->radius = value * this->radius;
}
