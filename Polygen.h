#pragma once
#ifndef POLYGEN_H
#define POLYGEN_H

#include <QPainter>
#include "Line.h"
#include <vector>
#include <stack>

class edge
{
private:
    double x;// 边的起始x坐标或边与扫描线的交点坐标
    double dx;// 边的斜率的倒数
    int ymax;// 边的最大y坐标
public:
    edge(){}
    edge(double x, double dx, int ymax)
        :x(x), dx(dx), ymax(ymax){}
    void setX(double x);
    void setDx(double dx);
    void setYmax(int ymax);
    double getX() const;
    double getDx() const;
    int getYmax() const;
};


class Polygen{
public:
    QPen pen;
    QPen fillpen;
    std::vector<QPoint> point;  //顶点集
    std::vector<Line> vertex;   //边集



public:
    Polygen(){}
    Polygen(std::vector<QPoint> p, QPen &pen_value);
    Polygen(const Polygen& p);
    bool need_fill; //是否填充
    //Polygen(const QPoint &cen, int r);

public:
    void draw(QPainter &painter);
    void setVertex();
    std::vector<QPoint> getPoint() const;
    void translate(int x, int y);
    void rotate(int x, int y, int dest_x, int dest_y);
    void scale(double value);
    void clear(QPainter &painter);
    void fill(QPainter &painter);
    void needfill();
    void setFillpen(QPen p);
    // void update();
    virtual ~Polygen();
};


#endif // POLYGEN_H
