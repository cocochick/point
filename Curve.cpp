#include "Curve.h"
#include <iostream>

struct Point{
    double x;
    double y;
};

Curve::Curve(std::vector<QPoint> p):control_point(p) {};

void Curve::draw(QPainter &painter){
    double t = 0;
    QPen pentmp = QPen(Qt::black,2);
    int n = control_point.size();
    for(auto x : control_point){
        Circle(x, 5, pentmp).draw(painter);
    }
    if(n <= 1)
        return;
    std::vector<Point> DeCasteljau;
    DeCasteljau.reserve(n);
    while(t <= 1){
        for(int i = 0; i < n; i++){
            DeCasteljau[i].x = control_point[i].x();
            DeCasteljau[i].y = control_point[i].y();
        }
        for(int k = 0; k < n - 1; k++){
            for(int i = 0; i < n - 1 - k; i++){
                DeCasteljau[i].x = (1 - t) * DeCasteljau[i].x + t * DeCasteljau[i + 1].x;
                DeCasteljau[i].y = (1 - t) * DeCasteljau[i].y + t * DeCasteljau[i + 1].y;
            }
        }
        painter.drawPoint(static_cast<int>(DeCasteljau[0].x + 0.5), static_cast<int>(DeCasteljau[0].y + 0.5));
        t += 0.0001;
    }
    /*for(int i = 0; i < control_point.size() - 1; i++){
        Line line(control_point[i], control_point[i + 1]);
        line.drawByBresenham(painter);
    }*/
}



void Curve::translate(int x, int y, int p){
    control_point[p].setX(x);
    control_point[p].setY(y);
}
