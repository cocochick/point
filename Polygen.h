#pragma once
#ifndef POLYGEN_H
#define POLYGEN_H

#include <QPainter>
#include "Line.h"
#include <vector>

class Polygen{
public:
    QPen pen;
    std::vector<QPoint> point;  //顶点集
    std::vector<Line> vertex;   //边集
public:
    Polygen(){}
    Polygen(std::vector<QPoint> p);
    Polygen(const Polygen& p);
    //Polygen(const QPoint &cen, int r);

public:
    void draw(QPainter &painter);
    void setVertex();
    std::vector<QPoint> getPoint() const;
    void translate(int x, int y);
    void rotate(int x, int y, int dest_x, int dest_y);
    void scale(double value);
    void clear(QPainter &painter);
};


#endif // POLYGEN_H
