#ifndef CURVE_H
#define CURVE_H

#include <QPainter>
#include "Line.h"
#include "Circle.h"
#include <vector>

class Curve{
public:
    std::vector<QPoint> control_point;  //控制点集
public:
    Curve(){}
    Curve(std::vector<QPoint> p);
    //Polygen(const QPoint &cen, int r);

public:
    void draw(QPainter &painter);
    void translate(int x, int y, int p);
};


#endif // CURVE_H
