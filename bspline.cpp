#include "BSpline.h"

BSpline::BSpline()
{

}

struct Point{
    double x;
    double y;
};

BSpline::BSpline(std::vector<QPoint> p): point(p){
    table.resize(p.size() + 3 + 1);     //阶数为3
    for(size_t i = 0; i < table.size(); i++){
        table[i] = i * 1.0 / (table.size() - 1);
    }

}


void BSpline::draw(QPainter &painter){
    double t = table[2];
    int n = point.size();
    func.clear();
    func.resize(n + 3);
    for(auto& x : func){
        x.resize(3);
    }
    for(auto x : point){
        Circle(x, 5).draw(painter);
    }
    if(n <= 1)
        return;
    while(t <= table[table.size() - 4]){
        //func.clear();
        for(int i = 0; i < n + 3; i++){
            func[i][0] = (table[i] <= t && t <= table[i + 1]) ? 1 : 0;
        }
        for(int j = 1; j < 3; j++){
            for(int i = 0; i < n + 2 - j; i++){
                func[i][j] = (t - table[i]) / (table[i + j] - table[i]) * func[i][j - 1]
                           + (table[i + j + 1] - t) / (table[i + j + 1] - table[i + 1]) * func[i + 1][j - 1];
            }
        }
        double x_sum = 0;
        double y_sum = 0;
        for(size_t i = 0; i < point.size(); i++){
            x_sum += func[i][2] * point[i].x();
            y_sum += func[i][2] * point[i].y();
        }
        painter.drawPoint(static_cast<int>(x_sum + 0.5), static_cast<int>(y_sum + 0.5));
            t += 0.0001;
    }
}
void BSpline::translate(int x, int y, int p){
    point[p].setX(x);
    point[p].setY(y);
}
