#ifndef BSPLINE_H
#define BSPLINE_H

#include <QPainter>
#include "Line.h"
#include "Circle.h"
#include <vector>
#include <Eigen/Dense>
#include<iostream>
class BSpline
{
public:
    std::vector<double> table;   //节点表
    std::vector<QPoint> point;  //控制点
    std::vector<std::vector<double>> func;   //基本函数表
public:
    BSpline();
    BSpline(std::vector<QPoint> p);
    void draw(QPainter &painter);
    void BSplineSet(QPainter &painter);
    void translate(int x, int y, int p);
    void BSplineFit(QPainter &painter);
};

#endif // BSPLINE_H
