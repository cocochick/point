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
    QPen pentmp = QPen(Qt::black,2);
    for(auto x : point){
        Circle(x, 5, pentmp).draw(painter);
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


void BSpline::BSplineSet(QPainter &painter) {
    int n = point.size();
    if(n < 5)
        return;
    Eigen::MatrixXd w = Eigen::MatrixXd::Zero(n, n);
    for (int i = 0; i < n; i++) {
            w(i, i) = 1;
            w(i, (i + 1) % n) = 4;
            w(i, (i + 2) % n) = 1;
    }
    Eigen::MatrixXd V = Eigen::MatrixXd::Zero(n, 2);
    for(int i = 0; i < n; i++){
        V(i, 0) = point[i].x();
        V(i, 1) = point[i].y();
    }
    V *= 6;
    Eigen::MatrixXd P = w.inverse() * V;
    for(int i = 0; i < n; i++){
        point[i].setX(P(i, 0));
        point[i].setY(P(i, 1));
    }
    BSplineFit(painter);
}

void BSpline::BSplineFit(QPainter &painter) {
    for (size_t i = 0; i < point.size(); i++) {
        QPoint xy[4];
        xy[0] = (point[i] + 4 * point[(i + 1) % point.size()] + point[(i + 2) % point.size()]) / 6;
        xy[1] = -(point[i] - point[(i + 2) % point.size()]) / 2;
        xy[2] = (point[i] - 2 * point[(i + 1) % point.size()] + point[(i + 2) % point.size()]) / 2;
        xy[3] = -(point[i] - 3 * point[(i + 1) % point.size()] + 3 * point[(i + 2) % point.size()] - point[(i + 3) % point.size()]) / 6;
        for (double t = 0; t <= 1; t += 0.001) {
            QPoint totalPoints = QPoint(0, 0);
            for (int j = 0; j < 4; j++) {
                totalPoints += xy[j] * pow(t, j);
            }
            painter.drawPoint(static_cast<int>(totalPoints.x() + 0.5), static_cast<int>(totalPoints.y() + 0.5));
        }
    }
}
