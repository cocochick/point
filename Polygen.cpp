#include "Polygen.h"

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



void Polygen::setVertex()
{
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
