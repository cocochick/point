#include "Polygen.h"
#include <iostream>

Polygen::Polygen(std::vector<QPoint> p):point(p) {
    for (auto iter = p.begin(); iter != p.end() - 1; iter++) {
        Line line(*iter, *(iter + 1));
        vertex.push_back(line);
    }
    Line line(*p.begin(), *(p.end() - 1));
    vertex.push_back(line);
}



void Polygen::draw(QPainter &painter){
    for (auto &line : vertex) {
        line.drawByBresenham(painter);
    }
}



void Polygen::setVertex(){   
    vertex.clear();
    for (auto iter = point.begin(); iter != point.end() - 1; iter++) {
        Line line(*iter, *(iter + 1));
        vertex.push_back(line);
    }
    Line line(*point.begin(), *(point.end() - 1));
    vertex.push_back(line);
}

std::vector<QPoint> Polygen::getPoint() const{
    return this->point;
}

void Polygen::translate(int x, int y){
    int trans_x = x - point[0].x();
    int trans_y = y - point[0].y();

    for(auto beg = point.begin();beg != point.end();++beg){
        beg->setX(beg->x() + trans_x);
        beg->setY(beg->y() + trans_y);
    }
    setVertex();
}
