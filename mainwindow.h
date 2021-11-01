#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <stack>
#include <set>
#include "Mode.h"
#include "Circle.h"
#include "Polygen.h"
#include "Curve.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct curPoint{
    curPoint():curve_num(-1),point_num(-1){};
    curPoint(int c, int p):curve_num(c),point_num(p){};
    int curve_num;
    int point_num;
};

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
    std::vector<Line> lines;
    std::vector<Polygen> polys;
    std::vector<Circle> circles;
    std::vector<Curve> curves;
    std::vector<QPoint> points;
    std::set<size_t> selected_line;
    std::set<size_t> selected_ploy;
    std::set<size_t> selected_circle;
    curPoint selected_curve;

private:
    void get_select(QPoint start, QPoint end);
    static bool inbox(QPoint p,QPoint left_up, QPoint right_down);
    void translate(QPoint dest);
    void rotate(QPoint base, QPoint dest);
    void select_draw(QPainter &painter);

public slots:
    //槽函数
    void setMode_Line();
    void setMode_Circle();
    void setMode_Pol();
    void setMode_Curve();
    void setMode_Select();
    void setMode_Move();
    void setMode_Rotate();
    void clearAll();


};
#endif // MAINWINDOW_H
