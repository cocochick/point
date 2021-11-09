#ifndef BSPLINEFIT_H
#define BSPLINEFIT_H

#include <QPainter>
#include "Line.h"
#include "Circle.h"
#include "BSpline.h"
#include <vector>
#include <Eigen/Dense>

class BSplineFit : public BSpline
{
public:
    BSplineFit();
    BSplineFit(std::vector<QPoint> p);
    void BSplineSet();
    void draw(QPainter &painter);
    void translate(int x, int y);

public:
    std::vector<QPoint> cpoint;  //控制点
};

#endif // BSPLINEFIT_H
