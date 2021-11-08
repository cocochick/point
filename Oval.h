#ifndef OVAL_H
#define OVAL_H
#include <QPainter>
#include <cmath>

class Oval{
private:
    QPen pen;
    QPoint center;
    int radius_x;
    int radius_y;
public:
    Oval(){};
    Oval(int xc, int yc, int r_x, int  r_y,  QPen &pen_value);
    Oval(QPoint start, QPoint end,QPen &pen_value);
    Oval(const Oval &o);

private:
    void midpoint(int xCtr, int yCtr, int rx, int ry,QPainter &painter) ;
    /*****************************
     * 由四分之一椭圆补全椭圆
     ****************************/
    void Complete(int x,int y,QPainter &painter) ;

public:
    void draw(QPainter &painter);
    QPoint getCenter();
    int getRadius_x();
    int getRadius_y();
    void translate(int x=0, int y=0);
    void rotate(int x, int y, int dest_x, int dest_y);
    void scale(double value);
    void clear(QPainter &painter);
};


#endif // OVAL_H
