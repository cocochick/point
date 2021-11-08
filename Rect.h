#ifndef RECT_H
#define RECT_H
#include "Polygen.h"

class Rect:public Polygen{
public:
    Rect(QPoint start, QPoint end,QPen &pen);
    Rect(Rect &r);
};

#endif // RECT_H
