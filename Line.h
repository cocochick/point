#pragma once
#ifndef LINE_H
#define LINE_H
#include <QPainter>


class Line :QPainter{
private:
    QPen pen;
    QPoint start;
    QPoint end;

public:

    Line();

    Line(int xS, int yS, int xE, int yE);
    Line(const Line& l);

    Line(QPoint s, QPoint e);

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

};






#endif // LINE_H
