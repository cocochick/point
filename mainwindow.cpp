#include  <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <cmath>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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
    scale_value = 1;
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
                Line(this->start,this->end).drawByBresenham(ptmp);
                pix = temppix;
                break;
            }
            case LINE:{
                Line(this->start,this->end).drawByBresenham(ptmp);
                break;
            }
            case CIRCLE:{
               int r =pow(pow(this->start.x()-this->end.x(),2)+pow(this->start.y()-this->end.y(),2),0.5);
               Circle(this->start,r).draw(ptmp);
               break;
            }
            case CURVE:{
                Curve(points).draw(ptmp);
                break;
            }
            case FILL:{
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
                select_draw(ptmp);
                break;

        }
        painter.drawPixmap(0, 0, temppix);
    }else{
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
                for(int i = 0; i < curves.size(); i++){
                    for(int j = 0; j < curves[i].control_point.size(); j++)
                        if(abs(tmp.x() - curves[i].control_point[j].x()) <= 10
                                && abs(tmp.y() - curves[i].control_point[j].y()) <= 10){
                            selected_curve = curPoint(i,j);
                            break;
                        }
                        else{
                            selected_curve.curve_num = -1;
                        }
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
            default:
                break;
        }
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
     switch(mode){
         case LINE:
         case SELECT:
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
         case FILL:{
            break;
         }
         case MOVE:{
            translate(e->pos());
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
                Line line(start,end);
                lines.push_back(line);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                break;
            }
            case CIRCLE:{
                int r =pow(pow(this->start.x()-this->end.x(),2)+pow(this->start.y()-this->end.y(),2),0.5);
                Circle cir(start,r);
                circles.push_back(cir);
                isDrawing = false;    //结束绘图
                push_leftbutton = false;
                this->update(this->rect());
                // std::cout<<"end"<<std::endl;
                break;
            }
            case POLYGEN:{
                // std::cout<<"3"<<std::endl;
                break;
            }
            case CURVE:{
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
                translate(e->pos());
                this->update(this->rect());
                selected_curve.curve_num = -1;
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
        switch(mode){
            case LINE:{
            }
            case CIRCLE:{
            }
            case POLYGEN:{
                Polygen pol(points);
                polys.push_back(pol);
                points.clear();
                push_leftbutton = false;
                isDrawing = false;
                this->update(this->rect());
                break;
            }
            case CURVE:{
            }
            case FILL:{
            }
            case SELECT:{
            }
            case MOVE:{
            }
            case SCALE:{
            }
            case ROTATE:{

            }
        }
    }
}


/********************************************************************
 ************************* 滚轮事件 **********************************
 *******************************************************************/
void MainWindow::wheelEvent(QWheelEvent *e){
    if(mode==SCALE){
        if(e->angleDelta().y() >0 ){
            if(scale_value <1) scale_value = 1;
            this->scale_value+=0.05;
            this->update(this->rect());
            this->scale(scale_value);
        }else{
            if(scale_value>1) scale_value = 1;
            if(scale_value-0.1 > 0.8){
                this->scale_value-=0.05;
            }
            this->update(this->rect());
            this->scale(scale_value);
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
}

void MainWindow::clearAll(){
    // mode =
    this->lines.clear();
    this->points.clear();
    this->circles.clear();
    this->polys.clear();
    this->curves.clear();
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    lines_tmp.clear();
    circles_tmp.clear();
    polys_tmp.clear();
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
    if(selected_curve.curve_num < 0)
        return;
    curves[selected_curve.curve_num].translate(dest.x(),dest.y(), selected_curve.point_num);

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
}

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

}


void MainWindow::select_draw(QPainter &painter){
    for(auto &index: selected_line){
        lines[index].drawByBresenham(painter);
    }
    for(auto &index: selected_circle){
        circles[index].draw(painter);
    }
    for(auto &index: selected_ploy){
        polys[index].draw(painter);
    }
    if(selected_curve.curve_num < 0)
        return;
    curves[selected_curve.curve_num].draw(painter);
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
    lines_tmp.clear();
    circles_tmp.clear();
    polys_tmp.clear();
}

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
}



