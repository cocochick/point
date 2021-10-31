#pragma once
#ifndef POLYGEN_H
#define POLYGEN_H

#include <QPainter>
#include "Line.h"
#include <vector>

class Polygen{
public:
    std::vector<QPoint> point;  //顶点集
    std::vector<Line> vertex;   //边集
public:
    Polygen(){}
    Polygen(std::vector<QPoint> p);
    //Polygen(const QPoint &cen, int r);

private:
public:
    void draw(QPainter &painter);
    void setVertex();
};


#endif // POLYGEN_H
