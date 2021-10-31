#include  <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"


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
            case POLYGEN:
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

            }
            case SCALE:{
            }
            case ROTATE:{

            }
        }
        painter.drawPixmap(0, 0, temppix);
    }else{
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
            case SELECT:
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
                    isDrawing = false;
                    this->update(this->rect());
                    std::cout<<"2"<<std::endl;
                }
                break;
            }
            case CURVE:{
            }
            case FILL:{
            }
            case MOVE:{
                this->tmp = e->pos();
                this->isDrawing = false;
                this->update(this->rect());
                break;
            }
            case SCALE:{
            }
            case ROTATE:{

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
                std::cout<<"moving"<<std::endl;
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
            this->isDrawing = true;
            this ->tmp = e->pos();
            this->update(this->rect());
            break;
         }
         case SCALE:{
         }
         case ROTATE:{

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
                std::cout<<"end"<<std::endl;
                break;
            }
            case POLYGEN:{
                std::cout<<"3"<<std::endl;
                break;
            }
            case CURVE:{
            }
            case FILL:{
            }
            case SELECT:{
                isDrawing = false;

                this->update(this->rect());
                break;
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
}

void MainWindow::setMode_Circle(){
    this->mode = CIRCLE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
}

void MainWindow::setMode_Pol(){
    this->mode = POLYGEN;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
}

void  MainWindow::setMode_Curve(){
    this->mode = CURVE;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
}
void  MainWindow::setMode_Select(){
    this->mode = SELECT;
    start = QPoint(0,0);
    end = QPoint(0,0);
    tmp = QPoint(0,0);
}

void MainWindow::clearAll(){
    this->lines.clear();
    this->points.clear();
    this->circles.clear();
    this->polys.clear();
    this->update(this->rect());
    pix = QPixmap(1000,800);
    pix.fill(Qt::white);
    temppix = pix;
    temppix.fill(Qt::white);
}

void MainWindow::get_select(QPoint left_up, QPoint right_down){
    for (auto beg =lines.begin();beg!=lines.end();++beg){

    }
    for (auto beg =circles.begin();beg!=circles.end();++beg){

    }
    for (auto beg =polys.begin();beg!=polys.end();++beg){

    }
}

