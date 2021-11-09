#include "BSplineFit.h"

BSplineFit::BSplineFit()
{

}

BSplineFit::BSplineFit(std::vector<QPoint> p){
    point = p;
    cpoint = p;
    table.resize(p.size() + 3 + 1);     //阶数为3
    for(size_t i = 0; i < table.size(); i++){
        table[i] = i * 1.0 / (table.size() - 1);
    }
}

void BSplineFit::BSplineSet() {
    int n = point.size();
    if(n < 4)
        return;
    cpoint = point;
    Eigen::MatrixXd w = Eigen::MatrixXd::Zero(n, n);
    for (int i = 1; i < n - 1; i++) {
            w(i, i - 1) = 1;
            w(i, i) = 4;
            w(i, i + 1) = 1;
    }
    w(0, 0) = 6;
    w(n - 1, n - 1) = 6;
    Eigen::MatrixXd V = Eigen::MatrixXd::Zero(n, 2);
    for(int i = 0; i < n; i++){
        V(i, 0) = point[i].x();
        V(i, 1) = point[i].y();
    }
    V *= 6;
    Eigen::MatrixXd P = w.inverse() * V;
    for(int i = 0; i < n; i++){
        cpoint[i].setX(P(i, 0));
        cpoint[i].setY(P(i, 1));
    }
    cpoint.insert(cpoint.begin(), QPoint(2 * cpoint[0] - cpoint[1]));
    cpoint.push_back(QPoint(2 * *(cpoint.end() - 1) - *(cpoint.end() - 2)));
}

void BSplineFit::draw(QPainter &painter) {
    QPen pentmp = QPen(Qt::black,2);
    for(auto x : point){
        Circle(x, 5, pentmp).draw(painter);
    }
    BSplineSet();
    if(point.size() < 4)
        return;
    for (size_t i = 0; i < cpoint.size() - 3; i++) {
        QPoint xy[4];
        xy[0] = (cpoint[i] + 4 * cpoint[(i + 1) % cpoint.size()] + cpoint[(i + 2) % cpoint.size()]) / 6;
        xy[1] = -(cpoint[i] - cpoint[(i + 2) % cpoint.size()]) / 2;
        xy[2] = (cpoint[i] - 2 * cpoint[(i + 1) % cpoint.size()] + cpoint[(i + 2) % cpoint.size()]) / 2;
        xy[3] = -(cpoint[i] - 3 * cpoint[(i + 1) % cpoint.size()] + 3 * cpoint[(i + 2) % cpoint.size()] - cpoint[(i + 3) % cpoint.size()]) / 6;
        for (double t = 0; t <= 1; t += 0.001) {
            QPoint totalPoints = QPoint(0.0, 0.0);
            for (int j = 0; j < 4; j++) {
                double tmp = 1;
                for(int te = 0; te < j; te++){
                    tmp *= t;
                }
                totalPoints += xy[j] * tmp;
            }
            painter.drawPoint(static_cast<int>(totalPoints.x() + 0.5), static_cast<int>(totalPoints.y() + 0.5));
        }
    }
}

void BSplineFit::translate(int x, int y){
    QPoint qpoint(0, 0);
    for(auto x : point){
        qpoint += x;
    }
    qpoint /= point.size();
    int trans_x = x - qpoint.x();
    int trans_y = y - qpoint.y();

    for(auto beg = point.begin();beg != point.end();++beg){
        beg->setX(beg->x() + x);
        beg->setY(beg->y() + y);
    }
}
