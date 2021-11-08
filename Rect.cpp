#include "Rect.h"

Rect::Rect(QPoint start, QPoint end,QPen &pen_value){
    point = std::vector<QPoint>{start,QPoint(start.x(),end.y()),end,QPoint(end.x(),start.y())};
    pen = pen_value;
    for (auto iter = point.begin(); iter != point.end() - 1; iter++) {
        Line line(*iter, *(iter + 1),pen_value);
        vertex.push_back(line);
    }
    Line line(*point.begin(), *(point.end() - 1),pen_value);
    vertex.push_back(line);
}
Rect::Rect(Rect &r){
    Polygen(r.point,r.pen);
}
