﻿#include "Line.h"
#include "method.h"
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace Eigen;

Line::Line(int xS, int yS, int xE, int yE,QPen &pen_value):
     start(xS, yS), end(xE, yE) {pen = pen_value;}

Line::Line(QPoint s, QPoint e, QPen &pen_value):
     start(s), end(e) {pen = pen_value;}

Line::Line(const Line& l) {
    pen = l.getpen();
    start = l.Start();
    end = l.End();
    pen = l.pen;
}

void Line::DDA(int xS, int yS, int xE, int yE,QPainter &painter) {
    int dx = xE - xS, dy = yE - yS, steps = 0;
    float xIncrement, yIncrement, x = static_cast<float>(xS), y = static_cast<float>(yS);
    steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    xIncrement = float(dx) / float(steps);
    yIncrement = float(dy) / float(steps);

    painter.drawPoint(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5));
    for (int i = 0; i < steps; ++i) {
        x += xIncrement;
        y += yIncrement;
        painter.drawPoint(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5));
    }
}

void Line::drawByDDA(QPainter &painter)  {
    DDA(start.x(), start.y(), end.x(), end.y(),painter);
}

void Line::MidPoint(int xS, int yS, int xE,
                    int yE,QPainter &painter) {
    if (xS > xE) {
        std::swap(xS, xE);
        std::swap(yS, yE);
    }
    int a = yS - yE, b = xE - xS, c = xS * yE - xE * yS;                                                                           //ֱ直线方程 50 50 -250
    int u = a * b > 0 ? 0
                      : 1;                                                                                                     //斜率大于0时u=1，反之=0
    int delta0 = u == 1 ? a + a + b : a + a - b, delta1 = u == 1 ? a + a : a + a - b - b, delta2 =
            u == 1 ? a + a + b + b : a + a; //变化量 50 0 100
    int x = xS, y = yS;                                                                                                            //当前点坐标

    painter.drawPoint(x, y);
    if (abs(a) <= abs(b)) { //|k|<=1
        while (x < xE) {
            if (delta0 < 0) {
                x++, y += u;
                delta0 += delta2;
            } else {
                x++, y += u - 1;
                delta0 += delta1;
            }
            painter.drawPoint(x, y);
        }
    } else {
        delta0 = u == 1 ? b + b + a : -b - b + a, delta1 = u == 1 ? b + b + a + a : -b - b, delta2 =
                u == 1 ? b + b : -b - b + a + a;
        int d = u == 1 ? 1 : -1;
        while (abs(y - yE) > 0) {
            if (delta0 < 0) { //k>0时x不动，k<0时x+1
                y += d, x += 1 - u;
                delta0 += delta2;
            } else {
                y += d, x += u;
                delta0 += delta1;
            }
            painter.drawPoint(x, y);
        }
    }
}

void Line::drawByMidPoint(QPainter &painter){
    MidPoint(start.x(), start.y(), end.x(), end.y(),painter);
}

void Line::Bresenham(int xS, int yS, int xE, int yE,QPainter &painter) {
    int dx = xE - xS, dy = yE - yS;
    int x = xS, y = yS;
    painter.drawPoint(xS, yS);
    int ux = dx > 0 ? 1 : -1;
    int uy = dy > 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int i = 0; i < abs(dx); ++i) {
            if (p < 0) {
                x += ux;
                p = p + 2 * dy;
            } else {
                x += ux;
                y += uy;
                p = p + 2 * dy - 2 * dx;
            }
            painter.drawPoint(x, y);
        }
    } else {
        int p = 2 * dx - dy;
        for (int i = 0; i < abs(dy); ++i) {
            if (p < 0) {
                y += uy;
                p = p + 2 * dx;
            } else {
                x += ux;
                y += uy;
                p = p + 2 * dx - 2 * dy;
            }
            painter.drawPoint(x, y);
        }
    }
}

void Line::drawByBresenham(QPainter &painter){
    painter.setPen(pen);
    painter.drawLine(start,end);
    //Bresenham(start.x(), start.y(), end.x(), end.y(),painter);
}


void Line::translate(int x, int y){
    int trans_x = x -(start.x()+end.x())/2;
    int trans_y = y - (start.y()+end.y())/2;
    start.setX(start.x()+x);
    end.setX(end.x()+x);
    start.setY(start.y()+y);
    end.setY(end.y()+y);
}

void Line::clear(QPainter &painter){
    QPen tmp = pen;
    pen.setColor(Qt::white);
    drawByBresenham(painter);
    pen = tmp;
}

void Line::rotate(int x, int y, int dest_x, int dest_y){
    int mid_x = (start.x()+end.x())/2;
    int mid_y = (start.y()+end.y())/2;
    double angle = get_angle(mid_x,mid_y,dest_x,dest_y,x,y);
    double tx = x; double ty = y;
    Matrix<double,3,3>  trans_org{
        {1, 0, tx},
        {0 ,1 ,ty},
        {0 ,0 ,1}};
    Matrix<double,3,3> rotate_M{
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0, 0, 1}};
    Matrix<double,3,3> trans_later{
        {1, 0, -tx},
        {0 ,1 ,-ty},
        {0 ,0 ,1}};
    // 旋转矩阵
    Matrix<double,3,3> M = trans_org * rotate_M * trans_later;
    // start
    Matrix<double,3, 1> org_start{(double)start.x(),(double)start.y(),1};
    Matrix<double,3,1> res_start = M * org_start;
    start.setX(int(res_start(0,0)+0.5));
    start.setY(int(res_start(1,0)+0.5));
    // end
    Matrix<double,3, 1> org_end{(double)end.x(),(double)end.y(),1};
    Matrix<double,3,1> res_end = M * org_end;
    end.setX(int(res_end(0,0)+0.5));
    end.setY(int(res_end(1,0)+0.5));

}

void Line::scale(double value){
    QPoint mid = (start+end)/2;
    start = value*(start - mid)+mid;
    end = value*(end-mid)+mid;
}
