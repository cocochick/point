#ifndef OVAL_H
#define OVAL_H
#include <QPainter>

class Oval{
private:
    QPainter painter;
    QPoint center;
    int radius_x;
    int radius_y;
public:
    Oval() = delete;
    Oval(int xc, int yc, int r_x, int  r_y, MainWindow* win);

private:
    void midpoint(int xCtr, int yCtr, int rx, int ry) ;
    /*****************************
     * 由四分之一椭圆补全椭圆
     ****************************/
    void Complete(int x,int y) ;

public:
    void draw(){
        midpoint(center.x(),center.y(),radius_x,radius_y);
    }
};

Oval::Oval(int xc, int yc, int r_x, int r_y,MainWindow* win)
    :painter(win), center(xc,yc), radius_x(r_x), radius_y(r_y){}

/* this version has too many float calculation
 **************************************************************
void Oval::midpoint(int xCtr, int yCtr, int rx, int ry) const {
    int x = 0, y = ry;
    float srx = float(rx)*float(rx);
    float sry = float(ry)*float(ry);
    float p1 = sry - 1.0*float(srx * ry) + 0.25f*srx;
    Complete(xCtr, yCtr + ry);
    while(sry*x<srx*y){
        if(p1<0){
            Complete(++x + xCtr, y + yCtr);
            p1 = p1 + sry*(2*float(x) + 1);
        }else{
            Complete(++x + xCtr, --y + yCtr);
            p1 = p1 + 2*sry * x - 2 * srx * y + sry;
        }
    }
    float p2 = sry*(x+0.5f)*(x+0.5f) + srx*(y-1)*(y-1) -sry*srx;
    while(y!=0){
        if(p2>0){
            Complete(x + xCtr, --y + yCtr);
            p2 = p2 + srx*(1 - 2*float(y));
        }else{
            Complete(++x + xCtr, --y + yCtr);
            p2 = p2 + 2*sry * x - 2 * srx * y + srx;
        }
    }
}
*************************************************************/

void Oval::midpoint(int xCtr, int yCtr, int rx, int ry) {
    int x = 0, y = ry;
    int srx = rx*rx;
    int sry = ry*ry;
    int p1 = sry - srx * ry + (srx >> 2);
    Complete(xCtr, yCtr + ry);
    while(sry*x<srx*y){
        if(p1<0){
            Complete(++x + xCtr, y + yCtr);
            p1 = p1 + sry*(2*x + 1);
        }else{
            Complete(++x + xCtr, --y + yCtr);
            p1 = p1 + 2*sry * x - 2 * srx * y + sry;
        }
    }
    int p2 = sry*(x*x+x)+ (sry>>2) + srx*(y-1)*(y-1) -sry*srx;
    while(y!=0){
        if(p2>0){
            Complete(x + xCtr, --y + yCtr);
            p2 = p2 + srx*(1 - 2*y);
        }else{
            Complete(++x + xCtr, --y + yCtr);
            p2 = p2 + 2*sry * x - 2 * srx * y + srx;
        }
    }
}



void Oval::Complete(int x, int y) {
    painter.drawPoint(x,y);
    painter.drawPoint(2*center.x()-x,y);
    painter.drawPoint(x,2*center.y()-y);
    painter.drawPoint(2*center.x()-x,2*center.y()-y);
}



#endif // OVAL_H
