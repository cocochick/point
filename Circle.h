#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <QPainter>

class Circle{
private:
    QPen pen;
    QPoint center;  //圆心
    int radius;   //半径
public:
    Circle();
    Circle(int x, int y, int r);
    Circle(const QPoint &cen, int r);
    Circle(const Circle& c);
    void translate(int x=0, int y=0);
    void clear(QPainter &painter);
    QPoint Center() const;

private:
    void midpoint(int xCtr, int yCtr,QPainter &painter);
    /****************************
     * 由八分之一圆补全到整个园
     ***************************/
    void Complete(int x, int y,QPainter &painter);

public:
    void draw(QPainter &painter);
    QPen getpen() const{return this->pen;};
    QPoint getCenter() const {return this->center;}  //圆心
    int getRadius()const {return this->radius;}   //半径
};




#endif // CIRCLE_H
