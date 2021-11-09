#include "Polygen.h"
#include "method.h"
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

Polygen::Polygen(std::vector<QPoint> p,QPen &pen_value):point(p) {
    this->need_fill = false;
    pen = pen_value;
    fillpen = pen;
    for (auto iter = p.begin(); iter != p.end() - 1; iter++) {
        Line line(*iter, *(iter + 1),pen_value);
        vertex.push_back(line);
    }
    Line line(*p.begin(), *(p.end() - 1),pen_value);
    vertex.push_back(line);

}


void Polygen::needfill(){
    this->need_fill = true;
}

void Polygen::setFillpen(QPen p){
    fillpen = p;
}

void Polygen::draw(QPainter &painter){
    painter.setPen(pen);
    for (auto &line : vertex) {
        line.drawByBresenham(painter);
    }
    if(need_fill){
        painter.setPen(fillpen);
        fill(painter);
    }
}

Polygen::Polygen(const Polygen& p){
    point = p.point;
    vertex = p.vertex;
    pen = p.pen;
    need_fill = p.need_fill;
    fillpen = p.fillpen;
    // update();
}

void Polygen::setVertex(){   
    vertex.clear();
    for (auto iter = point.begin(); iter != point.end() - 1; iter++) {
        Line line(*iter, *(iter + 1),pen);
        vertex.push_back(line);
    }
    Line line(*point.begin(), *(point.end() - 1),pen);
    vertex.push_back(line);
}

std::vector<QPoint> Polygen::getPoint() const{
    return this->point;
}

void Polygen::translate(int x, int y){
    int trans_x = x - point[0].x();
    int trans_y = y - point[0].y();

    for(auto beg = point.begin();beg != point.end();++beg){
        beg->setX(beg->x() + x);
        beg->setY(beg->y() + y);
    }
    setVertex();
}


void Polygen::rotate(int x, int y, int dest_x, int dest_y){
    double angle = get_angle(point[0].x(),point[0].y(),dest_x,dest_y,x,y);
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
    Matrix<double,3,3> M = trans_org * rotate_M * trans_later;
    for( auto &p: point){
        Matrix<double,3, 1> org{(double)p.x(),(double)p.y(),1};
        Matrix<double,3,1> res = M * org;
        p.setX(int(res(0,0)+0.5));
        p.setY(int(res(1,0)+0.5));
    }
    setVertex();
}

void Polygen::clear(QPainter &painter){
   for (auto &line : vertex) {
        line.clear(painter);
   }
}

void Polygen::scale(double value){
    QPoint mid;
    for(auto &p:point){
        mid+=p;
    }
    mid = mid/point.size();
    for(auto &p:point){
        p = value*(p-mid)+mid;
    }
    setVertex();
}

void Polygen::fill(QPainter &painter){
    point.push_back(point[0]);
        if(point.size()<3)
            return;
        int  j;
        int x0=0, x1=0, y=0, tx=0;

        int scanLines;//扫描线数量
        int min = point[0].y();//最低扫描线号
        int max = point[0].y();//最高扫描线号

        QVector<QList<edge>> ET;//边表
        QList<edge> AET;//活化边表
        QVector<double> arr;//扫描线与各边交点表

        QPoint p0;//边的起点
        QPoint p1;//边的终点
        QPoint temp;//保存位置变量
        edge* pNode;//边结点指针



        for (auto iterPos=point.begin(); iterPos!=point.end(); iterPos++)
            if (iterPos->y() < min)
                min = iterPos->y();
        for (auto iter=point.begin(); iter!=point.end(); iter++)
            if (iter->y() > max)
                max = iter->y();
        scanLines = max - min;

        //建立边表ET
        ET.resize(scanLines);
        //逐边进行处理，将每一条边的信息插入到ET中
        for(size_t i=0; i<point.size(); i++)
        {
            if(i < point.size()-1)
            {
                p0 = point[i];
                p1 = point[i+1];
            }
            else
            {
                p0 = point[i];
                p1 = point[0];
            }

            if(p0.y()>p1.y())//将p0设为边的起点坐标，y坐标较小
            {
                temp = p0;
                p0 = p1;
                p1 = temp;
            }

            if(p0.y() != p1.y())//非水平边
            {
                pNode = new edge;
                pNode->setX(static_cast<double>(p0.x()));
                pNode->setDx(static_cast<double>(p1.x()-p0.x())/(p1.y()-p0.y()));
                pNode->setYmax(p1.y()-1);//下闭上开
                ET[p0.y()-min].append(*pNode);
            }
        }//所有边都已插入到ET中

        for (int i=0; i<scanLines; i++)// 开始扫描，各边依次加入到AET中
        {
            y = i + min;//当前扫描线y坐标
            if(!ET[i].isEmpty())//有边加入AET
            {
                for(auto iterEdge=ET[i].begin(); iterEdge!=ET[i].end(); iterEdge++)
                    AET.append(*iterEdge);
            }
            ET[i].clear();//边结点已取出加入AET，无需保留

            //处理活化边表AET
            if(!AET.isEmpty())//首先删除扫描线以下的边
            {
                for(auto iterEdge=AET.begin(); iterEdge!=AET.end(); iterEdge++)
                {
                    if(AET.isEmpty())
                        break;
                    if(iterEdge->getYmax()<y){
                        AET.erase(iterEdge);
                        iterEdge=AET.begin();
               }
                }
            }

            if(!AET.isEmpty())//活化边表非空，求出交点，排序，画线
            {
                for(auto iterEdge=AET.begin(); iterEdge!=AET.end(); iterEdge++)
                {
                    arr.append(iterEdge->getX());//取出所有交点
                    iterEdge->setX(iterEdge->getX() + iterEdge->getDx());
                }
                std::sort(arr.begin(), arr.end());//交点排序
                for(j=0; j<arr.size(); j++)
                {
                    if(j%2 == 0 && j + 1 < arr.size())
                    {
                        tx = static_cast<int>(arr[j]);// 左边交点向右取整
                        if(arr[j]-tx)
                            x0 = tx + 1;
                        else
                            x0 = tx;
                        x1 = static_cast<int>(arr[j + 1]);// 右边交点向左取整
                    }
                    painter.drawLine(QPoint(x0, y),QPoint(x1, y));
                }
                arr.clear();
            }
        }//所有扫描线处理完毕
        ET.clear();
        point.pop_back();
}




Polygen::~Polygen(){

}

void edge::setX(double x)
{
    this->x = x;
}

void edge::setDx(double dx)
{
    this->dx = dx;
}

void edge::setYmax(int ymax)
{
    this->ymax = ymax;
}

double edge::getX() const
{
    return x;
}

double edge::getDx() const
{
    return dx;
}

int edge::getYmax() const
{
    return ymax;
}
