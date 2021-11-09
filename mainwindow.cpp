#include  <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <cmath>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QColorDialog>

bool inbox(QPoint p,QPoint left_up, QPoint right_down);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mode = CIRCLE;
    pix = QPixmap(1000,800);
    pix.fill(Qt::white);
    temppix = pix;
    temppix.fill();
    push_leftbutton = false;
    push_leftbutton_twice = false;
    need_clear = true;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    pen = QPen(Qt::black,2);
    move_start = QPoint(0, 0);
    // penwth设初值
    ui->penwth->setRange(1, 10);
    ui->penwth->setValue(2);


    //all in!!!!! ta ta kai
    connect(ui->Line, SIGNAL(clicked()), this, SLOT(setMode_Line()));
    connect(ui->Circle, SIGNAL(clicked()), this, SLOT(setMode_Circle()));
    connect(ui->Pol, SIGNAL(clicked()), this, SLOT(setMode_Pol()));
    connect(ui->Curve, SIGNAL(clicked()), this, SLOT(setMode_Curve()));
    connect(ui->select, SIGNAL(clicked()), this, SLOT(setMode_Select()));
    connect(ui->Delall, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->move, SIGNAL(clicked()), this, SLOT(setMode_Move()));
    connect(ui->rotate, SIGNAL(clicked()), this, SLOT(setMode_Rotate()));
    connect(ui->scale, SIGNAL(clicked()), this, SLOT(setMode_Scale()));
    connect(ui->bspline, SIGNAL(clicked()), this, SLOT(setMode_Bspline()));
    connect(ui->fit, SIGNAL(clicked()), this, SLOT(setMode_Fit()));
    connect(ui->penwth, SIGNAL(valueChanged(int)), this, SLOT(setValue_width(int)));
    connect(ui->pentype, SIGNAL(currentIndexChanged(int)), this, SLOT(setPen_type(int)));
    connect(ui->color, SIGNAL(clicked()), this, SLOT(setPen_pen()));
    connect(ui->rubber, SIGNAL(clicked()), this, SLOT(rubber()));
    connect(ui->rect, SIGNAL(clicked()), this, SLOT(setMode_Rect()));
    connect(ui->oval, SIGNAL(clicked()), this, SLOT(setMode_Oval()));
    connect(ui->Fill, SIGNAL(clicked()), this, SLOT(setMode_Fill()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
 //   painter.drawPixmap(0,0,pix);
    if(isDrawing){
        temppix = pix;
        QPainter ptmp(&temppix);
        switch(mode){
            case POLYGEN:{
                Line(this->start,this->end,pen).drawByBresenham(ptmp);
                pix = temppix;
                break;
            }
            case LINE:{
                Line(this->start,this->end,pen).drawByBresenham(ptmp);
                break;
            }
            case CIRCLE:{
               int r =pow(pow(this->start.x()-this->end.x(),2)+pow(this->start.y()-this->end.y(),2),0.5);
               Circle(this->start,r,pen).draw(ptmp);
               break;
            }
            case RECT:{
                Rect(this->start,this->end,pen).draw(ptmp);
                break;
            }
            case OVAL:{
                Oval(this->start,this->end,pen).draw(ptmp);
                break;
            }
            case CURVE:{
                Curve(points).draw(ptmp);
                break;
            }
            case BSPLINE:{
                BSpline(points).draw(ptmp);
                break;
            }
            case FIT:{
                BSplineFit(points).draw(ptmp);
                break;
            }
            case FILL:{
                break;
            }
            case SELECT:{
                ptmp.setPen(QPen(Qt::blue,1,Qt::DashDotLine));
                ptmp.drawRect(start.x(),start.y(),end.x()-start.x(),end.y()-start.y());
                break;
            }
            case MOVE:
                select_draw(ptmp);
                break;
            case ROTATE:
                select_draw_rotate(ptmp);
                break;
            case SCALE:
                if(need_clear){
                    select_clear_indarwimg(ptmp);
                    pix = temppix;
                    need_clear = false;
                }
                select_draw_scale(ptmp);
                break;

        }
        painter.drawPixmap(0, 0, temppix);
    }
    else{
        pix = QPixmap(1000,800);
        pix.fill(Qt::white);
        QPainter ppix(&pix);
        for (auto beg =lines.begin();beg!=lines.end();++beg){
            beg->drawByBresenham(ppix);
        }
        for (auto beg =circles.begin();beg!=circles.end();++beg){
            beg->draw(ppix);
        }
        for (auto beg =polys.begin();beg!=polys.end();++beg){
            beg->draw(ppix);
        }
        for (auto beg =curves.begin();beg!=curves.end();++beg){
            beg->draw(ppix);
        }
        for (auto beg =bsplines.begin();beg!=bsplines.end();++beg){
            beg->draw(ppix);
        }

        for (auto beg =fits.begin();beg!=fits.end();++beg){
            beg->draw(ppix);
        }

        for (auto beg =ovals.begin();beg!=ovals.end();++beg){
            beg->draw(ppix);
        }
        painter.drawPixmap(0, 0, pix);
    }
}

/**********************************************************************
 *********************** 鼠标事件 *************************************
 ********************************************************************/
void  MainWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        switch(mode){
            case LINE:
            case RECT:
            case OVAL:
            case CIRCLE:{
                this->start = e->pos();
                this->end = this->start;
                this->isDrawing = true;
                this->push_leftbutton = true;
                break;
            }
            case POLYGEN:{
                if(!push_leftbutton){
                    if(!points.empty()){points.clear();}
                    this->start = e->pos();
                    points.push_back(this->start);
                    this->tmp = this->start;
                    this->end = this->start;
                    this->isDrawing = true;
                    this->push_leftbutton = true;
                }else {
                    this ->start = this->end;
                    this ->end = e->pos();
                    points.push_back(this->end);
                    // isDrawing = false;
                    this->update(this->rect());
                    // std::cout<<"2"<<std::endl;
                }
                break;
            }
            case CURVE:{
                if(!push_leftbutton){
                    if(!points.empty()){points.clear();}
                    tmp = e->pos();
                    points.push_back(tmp);
                    isDrawing = true;
                    push_leftbutton = true;
                    this->update(this->rect());
                }
                else {
                    tmp = e->pos();
                    if(points.size() == 1){
                        points.push_back(tmp);
                    }
                    else{
                        points.insert(points.end() - 1, tmp);
                    }
                    // isDrawing = false;
                    this->update(this->rect());
                    // std::cout<<"2"<<std::endl;
                }
                break;
            }
            case BSPLINE:{
                if(!push_leftbutton){
                    if(!points.empty()){points.clear();}
                    tmp = e->pos();
                    points.push_back(tmp);
                    isDrawing = true;
                    push_leftbutton = true;
                    this->update(this->rect());
                }
                else {
                    tmp = e->pos();
                    points.push_back(tmp);
                    // isDrawing = false;
                    this->update(this->rect());
                    // std::cout<<"2"<<std::endl;
                }
                break;
            }
            case FIT:{
                if(!push_leftbutton){
                    if(!points.empty()){points.clear();}
                    tmp = e->pos();
                    points.push_back(tmp);
                    isDrawing = true;
                    push_leftbutton = true;
                    this->update(this->rect());
                }
                else {
                    tmp = e->pos();
                    points.push_back(tmp);
                    // isDrawing = false;
                    this->update(this->rect());
                    // std::cout<<"2"<<std::endl;
                }
                break;
            }
            case FILL:{
                break;
            }
            case SELECT:{
                this->selected_circle.clear();
                this->selected_line.clear();
                this->selected_ploy.clear();
                this->start = e->pos();
                this->end = this->start;
                this->isDrawing = true;
                this->push_leftbutton = true;
                break;
            }
            case MOVE:{
                this->isDrawing = true;
                tmp = e->pos();
                if(move_start == QPoint(0, 0))
                    move_start = tmp;
                if(selected_bsplines.empty() && selected_circle.empty() && selected_curves.empty() && selected_fits.empty()
                        && selected_line.empty() && selected_oval.empty() && selected_ploy.empty()){
                    for(size_t i = 0; i < curves.size(); i++){
                        for(size_t j = 0; j < curves[i].control_point.size(); j++)
                            if(abs(tmp.x() - curves[i].control_point[j].x()) <= 10
                                    && abs(tmp.y() - curves[i].control_point[j].y()) <= 10){
                                selected_curve = curPoint(i,j);
                                break;
                            }
                            else{
                                selected_curve.curve_num = -1;
                            }
                    }
                    for(size_t i = 0; i < bsplines.size(); i++){
                        for(size_t j = 0; j < bsplines[i].point.size(); j++)
                            if(abs(tmp.x() - bsplines[i].point[j].x()) <= 10
                                    && abs(tmp.y() - bsplines[i].point[j].y()) <= 10){
                                selected_bspline = curPoint(i,j);
                                break;
                            }
                            else{
                                selected_bspline.curve_num = -1;
                            }
                    }
                }
                else{
                    selected_curve.curve_num = -1;
                    selected_bspline.curve_num = -1;
                }
                this->update(this->rect());
                break;
            }
            case SCALE:{
                break;
            }
            case ROTATE:{
                if(!push_leftbutton){
                    this->isDrawing = true;
                    this->push_leftbutton = true;
                    this->start = e->pos();
                    this->push_leftbutton_twice = false;
                    //this->end = this->start;
                    //this->update(this->rect());
                }else{
                    this->push_leftbutton_twice = true;
                    this->isDrawing = true;
                    this->end = e->pos();
                    rotate(start,end);
                    this->update(this->rect());
                }
                break;
            }
        }
     }
    else if(e->button()==Qt::RightButton){
        switch(mode){
            case CURVE:{
                Curve cur(points);
                curves.push_back(cur);
                points.clear();
                push_leftbutton = false;
                isDrawing = false;
                this->update(this->rect());
                break;
            }
            case BSPLINE:{
                BSpline bsp(points);
                bsplines.push_back(bsp);
                points.clear();
                push_leftbutton = false;
                isDrawing = false;
                this->update(this->rect());
                break;
            }
            case FIT:{
                BSplineFit bspf(points);
                fits.push_back(bspf);
                points.clear();
                push_leftbutton = false;
                isDrawing = false;
                this->update(this->rect());
                break;
            }
            default:
                break;
        }
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    switch(mode){
        case LINE:
        case SELECT:
        case RECT:
        case OVAL:
        case CIRCLE:{
            if(push_leftbutton){
                this->end = e->pos();
                isDrawing = true;
                this->update(this->rect());
                // std::cout<<"moving"<<std::endl;
            }
            break;
        }
        case POLYGEN:{
            break;
        }
        case CURVE:{
            break;
        }
        case BSPLINE:{
            break;
        }
        case FIT:{
            break;
        }
        case FILL:{
            break;
        }
        case MOVE:{
            translate_drawing(e->pos() - move_start);
            this->update(this->rect());
            break;
        }
        case SCALE:{
        }
        case ROTATE:{
            if(push_leftbutton_twice){
                rotate_drawing(start,e->pos());
                this->update(this->rect());
            }
            break;
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton) //鼠标左键释放
      {
        switch(mode){
            case LINE:{
                Line line(start,end,pen);
                lines.push_back(line);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                break;
            }
            case CIRCLE:{
                int r =pow(pow(this->start.x()-this->end.x(),2)+pow(this->start.y()-this->end.y(),2),0.5);
                Circle cir(start,r,pen);
                circles.push_back(cir);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                // std::cout<<"end"<<std::endl;
                break;
            }
            case RECT:{
                Rect rect(start,end,pen);
                polys.push_back(rect);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                break;
            }
            case OVAL:{
                Oval oval(start,end,pen);
                ovals.push_back(oval);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                break;
            }
            case POLYGEN:{
                // std::cout<<"3"<<std::endl;
                break;
            }
            case CURVE:{
                break;
            }
            case BSPLINE:{
                break;
            }
            case FIT:{
                break;
            }
            case FILL:{
                break;
            }
            case SELECT:{
                isDrawing = false;
                this->push_leftbutton = false;
                get_select(start,end);
                // std::cout<<selected_line.size()<<std::endl<<selected_circle.size()<<std::endl<<selected_ploy.size()<<std::endl;
                this->update(this->rect());
                break;
            }
            case MOVE:{
                this->isDrawing = false;
                // need_clear = true;
                translate(e->pos() - move_start);
                move_start = QPoint(0, 0);
                this->update(this->rect());
                selected_curve.curve_num = -1;
                selected_bspline.curve_num = -1;
                break;
            }
            case SCALE:{
                break;
            }
            case ROTATE:{
                if(push_leftbutton_twice){
                    // need_clear = true;
                    this->isDrawing = false;
                    this->push_leftbutton = false;
                    this->push_leftbutton_twice = false;
                    rotate(start,e->pos());
                    this->update(this->rect());
                }
                break;
            }
        }
      }
}

void  MainWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        if(mode == POLYGEN){
             Polygen pol(points,pen);
             polys.push_back(std::move(pol));
             points.clear();
             push_leftbutton = false;
             isDrawing = false;
             this->update(this->rect());
        }
   }
}


/********************************************************************
 ************************* 滚轮事件 **********************************
 *******************************************************************/
void MainWindow::wheelEvent(QWheelEvent *e){
    if(mode==SCALE){
        if(e->angleDelta().y() >0 ){
            this->update(this->rect());
            this->scale(1.05);
        }else{
            this->update(this->rect());
            this->scale(0.95);
        }
    }
}

/******************************************************************
 ******************* 槽函数 ***************************************
 *****************************************************************/
void MainWindow::setMode_Line(){
    this->mode = LINE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::setMode_Circle(){
    this->mode = CIRCLE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::setMode_Pol(){
    this->mode = POLYGEN;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void  MainWindow::setMode_Curve(){
    this->mode = CURVE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}
void  MainWindow::setMode_Select(){
    this->mode = SELECT;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::clearAll(){
    // mode =
    this->lines.clear();
    this->points.clear();
    this->circles.clear();
    this->polys.clear();
    this->curves.clear();
    this->bsplines.clear();
    this->fits.clear();
    this->ovals.clear();
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
    lines_tmp.clear();
    circles_tmp.clear();
    polys_tmp.clear();
    ovals_tmp.clear();
    this->isDrawing = false;
    pix = QPixmap(1000,800);
    pix.fill(Qt::white);
    temppix = pix;
    temppix.fill(Qt::white);
    this->update(this->rect());
}

void MainWindow::setMode_Move(){
    this->mode = MOVE;
    this->isDrawing = false;
}

void MainWindow::setMode_Rotate(){
    this->mode = ROTATE;
}

void MainWindow::setMode_Scale(){
    this->mode = SCALE;
    this->need_clear = true;
    this->isDrawing = true;
}

void MainWindow::setMode_Bspline(){
    this->mode = BSPLINE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::setMode_Fit(){
    this->mode = FIT;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}


void MainWindow::setValue_width(int value){
    if(value<0) value =1;
    pen.setWidth(value);
}

void MainWindow::setPen_type(int index){
    if(index == 0){
        pen.setStyle(Qt::SolidLine);
    }else if(index == 1){
        pen.setStyle(Qt::DashLine);
    }else if(index == 2){
        pen.setStyle(Qt::DotLine);
    }
}

void MainWindow::setPen_pen(){
    QColor color = QColorDialog::getColor(Qt::black, this);
    pen.setColor(color);
}

void MainWindow::rubber(){
     for(auto &index: selected_line){
         lines[index]=Line(0,0,0,0,pen);
     }
        for(auto &index: selected_circle){
         circles[index] = Circle(0,0,0,pen);
     }
     for(auto &index: selected_ploy){
         polys[index] = Polygen(std::vector<QPoint>{QPoint(0,0)}, pen);
     }
     for(auto &index: selected_oval){
         ovals[index] = Oval(QPoint(0,0),QPoint(0,0), pen);
     }
     this->update(this->rect());
}

void MainWindow::setMode_Rect(){
    this->mode = RECT;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::setMode_Oval(){
    this->mode = OVAL;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->isDrawing = false;
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->selected_oval.clear();
    this->selected_bsplines.clear();
    this->selected_curves.clear();
    this->selected_fits.clear();
}

void MainWindow::setMode_Fill(){
    for(auto &index: selected_ploy){
        polys[index].setFillpen(QPen(pen.color(),1));
        polys[index].needfill();
    }
    this->update(this->rect());
}
/********************************************************************************
 **************************** 选区 ***********************************************
 ********************************************************************************/

void MainWindow::get_select(QPoint left_up, QPoint right_down){
    size_t i = 0;
    for (auto beg =lines.begin();beg!=lines.end();++beg){
        if(inbox(beg->End(),left_up,right_down) && inbox(beg->Start(),left_up,right_down)){
            selected_line.insert(i);
        }
        ++i;
    }
    i = 0;
    for (auto beg =circles.begin();beg!=circles.end();++beg){
        if(inbox(beg->getCenter(),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(0,beg->getRadius()),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(0,-beg->getRadius()),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(beg->getRadius(),0),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(-beg->getRadius(),0),left_up,right_down)){
            selected_circle.insert(i);
        }
        ++i;
    }
    i = 0;
    for (auto beg =polys.begin();beg!=polys.end();++beg){
        auto point_tmp = beg->getPoint();
        bool in_box = true;
        for(auto pbeg = point_tmp.begin();pbeg != point_tmp.end(); ++pbeg){
            if(!inbox(*pbeg,left_up,right_down)){
                in_box = false;
                break;
            }
        }
        if(in_box) selected_ploy.insert(i);
        ++i;
    }
    i = 0;
    for (auto beg =ovals.begin();beg!=ovals.end();++beg){
        if(inbox(beg->getCenter(),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(0,beg->getRadius_y()),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(0,-beg->getRadius_y()),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(beg->getRadius_x(),0),left_up,right_down)
                && inbox(beg->getCenter()+QPoint(-beg->getRadius_x(),0),left_up,right_down)){
            selected_oval.insert(i);
        }
        ++i;
    }
    i = 0;
    for (auto beg =curves.begin();beg!=curves.end();++beg){
        bool selected = true;
        for(auto p : beg->control_point){
            if(!inbox(p,left_up,right_down) || !inbox(p,left_up,right_down)){
                selected = false;
                break;
            }
        }
        if(selected){
            selected_curves.insert(i);
        }
        ++i;
    }
    i = 0;
    for (auto beg =bsplines.begin();beg!=bsplines.end();++beg){
        bool selected = true;
        for(auto p : beg->point){
            if(!inbox(p,left_up,right_down) || !inbox(p,left_up,right_down)){
                selected = false;
                break;
            }
        }
        if(selected){
            selected_bsplines.insert(i);
        }
        ++i;
    }
    i = 0;
    for (auto beg =fits.begin();beg!=fits.end();++beg){
        bool selected = true;
        for(auto p : beg->point){
            if(!inbox(p,left_up,right_down) || !inbox(p,left_up,right_down)){
                selected = false;
                break;
            }
        }
        if(selected){
            selected_fits.insert(i);
        }
        ++i;
    }
}

bool MainWindow::inbox(QPoint p,QPoint left_up, QPoint right_down){
    if(p.x()>left_up.x() && p.x()<right_down.x() && p.y()>left_up.y() && p.y()<right_down.y()){
        return true;
    }
    return false;
}

/****************************************************************************
 ******************************* 移动函数 ************************************
 ***************************************************************************/

void MainWindow::translate(QPoint dest){
    for(auto &index: selected_line){
        lines[index].translate(dest.x(),dest.y());
    }
    for(auto &index: selected_circle){
        circles[index].translate(dest.x(),dest.y());
    }
    for (auto &index: selected_ploy){
        polys[index].translate(dest.x(),dest.y());
    }
    for (auto &index: selected_oval){
        ovals[index].translate(dest.x(),dest.y());
    }
    for (auto &index: selected_curves){
        curves[index].translate(dest.x(),dest.y());
    }
    for (auto &index: selected_bsplines){
        bsplines[index].translate(dest.x(),dest.y());
    }
    for (auto &index: selected_fits){
        fits[index].translate(dest.x(),dest.y());
    }
    if(selected_curve.curve_num >= 0)
        curves[selected_curve.curve_num].translate(dest.x() + move_start.x(), dest.y() + move_start.y(), selected_curve.point_num);
    if(selected_bspline.curve_num >= 0)
        bsplines[selected_bspline.curve_num].translate(dest.x() + move_start.x(), dest.y() + move_start.y(), selected_bspline.point_num);
    /*
    if(selected_curve.curve_num < 0)
        return;
    curves[selected_curve.curve_num].translate(dest.x(),dest.y(), selected_curve.point_num);
    */
}

void MainWindow::translate_drawing(QPoint base){
    for(auto &index: selected_line){
        Line line_tmp = lines[index];
        line_tmp.translate(base.x(),base.y());
        lines_tmp.push_back(line_tmp);
    }
    for(auto &index: selected_circle){
        Circle cir_tmp = circles[index];
        cir_tmp.translate(base.x(),base.y());
        circles_tmp.push_back(cir_tmp);
    }
    for(auto &index: selected_ploy){
        Polygen pol_tmp = polys[index];
        pol_tmp.translate(base.x(),base.y());
        polys_tmp.push_back(pol_tmp);
    }
    for(auto &index: selected_oval){
        Oval ovl_tmp = ovals[index];
        ovl_tmp.translate(base.x(),base.y());
        ovals_tmp.push_back(ovl_tmp);
    }
    for (auto &index: selected_curves){
        Curve cur_tmp = curves[index];
        cur_tmp.translate(base.x(),base.y());
        curves_tmp.push_back(cur_tmp);
    }
    for (auto &index: selected_bsplines){
        BSpline bsp_tmp = bsplines[index];
        bsp_tmp.translate(base.x(),base.y());
        bsplines_tmp.push_back(bsp_tmp);
    }
    for (auto &index: selected_fits){
        BSplineFit fit_tmp = fits[index];
        fit_tmp.translate(base.x(),base.y());
        fits_tmp.push_back(fit_tmp);
    }
    if(selected_curve.curve_num >= 0)
        curves[selected_curve.curve_num].translate(base.x() + move_start.x(), base.y() + move_start.y(), selected_curve.point_num);
    if(selected_bspline.curve_num >= 0)
        bsplines[selected_bspline.curve_num].translate(base.x() + move_start.x(), base.y() + move_start.y(), selected_bspline.point_num);

}

/****************************************************************************
 ******************************* 旋转 ***************************************
 ***************************************************************************/
void MainWindow::rotate(QPoint base, QPoint dest){
    for(auto &index: selected_line){
        lines[index].rotate(base.x(),base.y(),dest.x(),dest.y());
    }
    for(auto &index: selected_circle){
        circles[index].rotate(base.x(),base.y(),dest.x(),dest.y());
    }
    for(auto &index: selected_ploy){
        polys[index].rotate(base.x(),base.y(),dest.x(),dest.y());
    }
    for(auto &index: selected_oval){
        ovals[index].rotate(base.x(),base.y(),dest.x(),dest.y());
    }
}

void MainWindow::rotate_drawing(QPoint base, QPoint dest){
    for(auto &index: selected_line){
        Line line_tmp = lines[index];
        line_tmp.rotate(base.x(),base.y(),dest.x(),dest.y());
        lines_tmp.push_back(line_tmp);
    }
    for(auto &index: selected_circle){
        Circle cir_tmp = circles[index];
        cir_tmp.rotate(base.x(),base.y(),dest.x(),dest.y());
        circles_tmp.push_back(cir_tmp);
    }
    for(auto &index: selected_ploy){
        Polygen pol_tmp = polys[index];
        pol_tmp.rotate(base.x(),base.y(),dest.x(),dest.y());
        polys_tmp.push_back(pol_tmp);
    }
    for(auto &index: selected_oval){
        Oval ovl_tmp = ovals[index];
        ovl_tmp.rotate(base.x(),base.y(),dest.x(),dest.y());
        ovals_tmp.push_back(ovl_tmp);
    }
}

/****************************************************************************
 ******************************* 缩放 ***************************************
 ***************************************************************************/

void MainWindow::scale(double value){
    for(auto &index: selected_circle){
        circles[index].scale(value);
    }
    for(auto &index: selected_line){
        lines[index].scale(value);
    }
    for(auto &index: selected_ploy){
        polys[index].scale(value);
    }
    for(auto &index: selected_oval){
        ovals[index].scale(value);
    }

}


/****************************************************************************
 ******************************* 缓冲区绘制 ***********************************
 ***************************************************************************/
void MainWindow::select_draw_scale(QPainter &painter){
    for(auto &index: selected_line){
        lines[index].drawByBresenham(painter);
    }
    for(auto &index: selected_circle){
        circles[index].draw(painter);
    }
    for(auto &index: selected_ploy){
        polys[index].draw(painter);
    }
    for(auto &index: selected_oval){
        ovals[index].draw(painter);
    }
    for(auto &index: selected_curves){
        curves[index].draw(painter);
    }
    for(auto &index: selected_bsplines){
        bsplines[index].draw(painter);
    }
    for(auto &index: selected_fits){
        fits[index].draw(painter);
    }
}

void MainWindow::select_draw(QPainter &painter){
    for(auto &index: lines_tmp){
        index.drawByBresenham(painter);
    }
    for(auto &index: circles_tmp){
        index.draw(painter);
    }
    for(auto &index: polys_tmp){
        index.draw(painter);
    }
    for(auto &index: ovals_tmp){
        index.draw(painter);
    }
    for(auto &index: curves_tmp){
        index.draw(painter);
    }
    for(auto &index: bsplines_tmp){
        index.draw(painter);
    }
    for(auto &index: fits_tmp){
        index.draw(painter);
    }
    if(selected_curve.curve_num >= 0)
        curves[selected_curve.curve_num].draw(painter);
    if(selected_bspline.curve_num >= 0)
        bsplines[selected_bspline.curve_num].draw(painter);
    lines_tmp.clear();
    circles_tmp.clear();
    polys_tmp.clear();
    ovals_tmp.clear();
    curves_tmp.clear();
    bsplines_tmp.clear();
    fits_tmp.clear();
}

void MainWindow::select_draw_rotate(QPainter &painter){
    for(auto &line :lines_tmp){
        line.drawByBresenham(painter);
    }
    for(auto &cir :circles_tmp){
        cir.draw(painter);
    }
    for(auto &poly:polys_tmp){
        poly.draw(painter);
    }
    for(auto &ovl:ovals_tmp){
        ovl.draw(painter);
    }
    lines_tmp.clear();
    circles_tmp.clear();
    polys_tmp.clear();
    ovals_tmp.clear();
}

/****************************************************************************
 ******************************* 缓冲区清除 ***********************************
 ***************************************************************************/
void MainWindow::select_clear_indarwimg(QPainter &painter){
    for(auto &index: selected_line){
        lines[index].clear(painter);
    }
    for(auto &index: selected_circle){
        circles[index].clear(painter);
    }
    for(auto &index: selected_ploy){
        polys[index].clear(painter);
    }
    for(auto &index: selected_oval){
        ovals[index].clear(painter);
    }



}



