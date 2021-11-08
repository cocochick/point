#pragma once
#ifndef LINE_H
#define LINE_H
#include <QPainter>


class Line{
private:
    QPen pen;
    QPoint start;
    QPoint end;

public:

    Line();

    Line(int xS, int yS, int xE, int yE , QPen &pen_value);
    Line(const Line& l);
    Line(QPoint s, QPoint e, QPen &pen_value);

    void set( QPoint s, QPoint e) {
        start = s;
        end = e;
    }

private:
    void DDA(int xS, int yS, int xE, int yE,QPainter &painter);

    void MidPoint(int xS, int yS, int xE, int yE,QPainter &painter);

    void Bresenham(int xS, int yS, int xE, int yE,QPainter &painter);

public:
    void drawByDDA(QPainter &painter);

    void drawByMidPoint(QPainter &painter);

    void drawByBresenham(QPainter &painter);
    QPen getpen() const{ return this->pen;}
    QPoint Start() const{return this->start;}
    QPoint End() const{return this->end;}

    void translate(int x=0, int y=0);
    void rotate(int x, int y, int dest_x, int dest_y);
    void scale(double value);
    void clear(QPainter &painter);

};






#endif // LINE_H
