#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <stack>
#include <set>
#include "Mode.h"
#include "Circle.h"
#include "Polygen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

private:
    // std::vector<QPoint> points;
    QPoint start;
    QPoint end;
    QPoint tmp;
    Ui::MainWindow *ui;
    Mode mode;
    QPixmap pix;
    QPixmap temppix; //双缓冲
    bool isDrawing; // 正在绘图
    bool push_leftbutton;
    bool push_leftbutton_twice;
    bool need_clear; //是否需要清除
    std::vector<Line> lines;
    std::vector<Polygen> polys;
    std::vector<Circle> circles;
    std::vector<QPoint> points;
    std::set<size_t> selected_line;
    std::set<size_t> selected_ploy;
    std::set<size_t> selected_circle;
    float scale_value; // 缩放系数

private:
    void get_select(QPoint start, QPoint end);
    static bool inbox(QPoint p,QPoint left_up, QPoint right_down);
    void translate(QPoint dest);
    void rotate(QPoint base, QPoint dest);
    void scale(double value);
    void select_draw(QPainter &painter);
    void select_clear_indarwimg(QPainter &painter);

public slots:
    //槽函数
    void setMode_Line();
    void setMode_Circle();
    void setMode_Pol();
    void setMode_Curve();
    void setMode_Select();
    void setMode_Move();
    void setMode_Rotate();
    void setMode_Scale();
    void clearAll();


};
#endif // MAINWINDOW_H
