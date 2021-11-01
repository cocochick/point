#include  <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    //all in!!!!! ta ta kai
    connect(ui->Line, SIGNAL(clicked()), this, SLOT(setMode_Line()));
    connect(ui->Circle, SIGNAL(clicked()), this, SLOT(setMode_Circle()));
    connect(ui->Pol, SIGNAL(clicked()), this, SLOT(setMode_Pol()));
    connect(ui->Curve, SIGNAL(clicked()), this, SLOT(setMode_Curve()));
    connect(ui->select, SIGNAL(clicked()), this, SLOT(setMode_Select()));
    connect(ui->Delall, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->move, SIGNAL(clicked()), this, SLOT(setMode_Move()));
    connect(ui->rotate, SIGNAL(clicked()), this, SLOT(setMode_Rotate()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
   // painter.drawPixmap(0,0,pix);
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
            }
            case FILL:{
            }
            case SELECT:{
                ptmp.setPen(QPen(Qt::blue,1,Qt::DashDotLine));
                ptmp.drawRect(start.x(),start.y(),end.x()-start.x(),end.y()-start.y());
                break;
            }
            case MOVE:{
                select_draw(ptmp);
            }
            case SCALE:{
            }
            case ROTATE:{
                select_draw(ptmp);
            }
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
            }
            case FILL:{
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
                this->update(this->rect());
                break;
            }
            case SCALE:{
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
             //this ->start = this->end;
             //this->end = this->tmp;
             //push_leftbutton = false;
             //isDrawing = false;
             //this->update(this->rect());
             //break;
         }
        case CURVE:{
        }
         case FILL:{
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
                rotate(start,e->pos());
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
            }
            case FILL:{
            }
            case SELECT:{
                isDrawing = false;
                this->push_leftbutton = false;
                get_select(start,end);
                std::cout<<selected_line.size()<<std::endl<<selected_circle.size()<<std::endl<<selected_ploy.size()<<std::endl;
                this->update(this->rect());
                break;
            }
            case MOVE:{
                this->isDrawing = false;
                translate(e->pos());
                this->update(this->rect());
                break;
            }
            case SCALE:{
            }
            case ROTATE:{
                if(push_leftbutton_twice){
                    this->isDrawing = false;
                    this->push_leftbutton = false;
                    this->push_leftbutton_twice = false;
                    //rotate(start,e->pos());
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

/******************************************************************
 ******************* 槽函数 ***************************************
 *****************************************************************/
void MainWindow::setMode_Line(){
    this->mode = LINE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
}

void MainWindow::setMode_Circle(){
    this->mode = CIRCLE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
}

void MainWindow::setMode_Pol(){
    this->mode = POLYGEN;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
}

void  MainWindow::setMode_Curve(){
    this->mode = CURVE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
}
void  MainWindow::setMode_Select(){
    this->mode = SELECT;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
}

void MainWindow::clearAll(){
    this->lines.clear();
    this->points.clear();
    this->circles.clear();
    this->polys.clear();
    this->selected_circle.clear();
    this->selected_line.clear();
    this->selected_ploy.clear();
    this->update(this->rect());
    pix = QPixmap(1000,800);
    pix.fill(Qt::white);
    temppix = pix;
    temppix.fill(Qt::white);
}

void MainWindow::setMode_Move(){
    this->mode = MOVE;
}

void MainWindow::setMode_Rotate(){
    this->mode = ROTATE;
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
}

/****************************************************************************
 ******************************* 旋转 ************************************
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
}


