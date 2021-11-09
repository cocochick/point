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


/*
void BSpline::draw(QPainter &painter){
    double t = table[2];
    double t_end = table[table.size() - 4];
    int n = point.size();
    func.clear();
    func.resize(n + 3);
    for(auto& x : func){
        x.resize(3);
    }
    for(auto x : point){
        Circle(x, 15).draw(painter);
    }
    if(n <= 2)
        return;
    //Eigen::MatrixX<double> A(n, n);
    Eigen::MatrixX<double> A(static_cast<int>((t_end - t) / 0.001 + 1), n);
    Eigen::MatrixX<double> q_x(n, 1);
    Eigen::MatrixX<double> p_x(n, 1);
    Eigen::MatrixX<double> p_temp_x(n, 1);
    Eigen::MatrixX<double> delta_x(n, 1);
    Eigen::MatrixX<double> delta_temp_x(n, 1);
    Eigen::MatrixX<double> q_y(n, 1);
    Eigen::MatrixX<double> p_y(n, 1);
    Eigen::MatrixX<double> p_temp_y(n, 1);
    Eigen::MatrixX<double> delta_y(n, 1);
    Eigen::MatrixX<double> delta_temp_y(n, 1);
    for(int i = 0; i < n; i++){
        p_x(i, 0) = point[i].x();
        p_y(i, 0) = point[i].y();
        q_x(i, 0) = point[i].x();
        q_y(i, 0) = point[i].y();
    }
    int i_A = 0;
    while(t <= t_end){
        for(int i = 0; i < n + 3; i++){
            func[i][0] = (table[i] <= t && t <= table[i + 1]) ? 1 : 0;
        }
        for(int j = 1; j < 3; j++){
            for(int i = 0; i < n + 2 - j; i++){
                func[i][j] = (t - table[i]) / (table[i + j] - table[i]) * func[i][j - 1]
                           + (table[i + j + 1] - t) / (table[i + j + 1] - table[i + 1]) * func[i + 1][j - 1];
            }
        }
        for(int i = 0; i < n; i++){
            A(i_A , i) = func[i][2];
        }
        i_A++;
        //t += (t_end - table[2]) / (point.size() - 1);
        t += 0.001;
    }
    for(int t = 0; t < 100; t++){
        Eigen::MatrixX<double> temp_x(static_cast<int>((t_end - table[2]) / 0.001 + 1), 1);
        Eigen::MatrixX<double> temp_y(static_cast<int>((t_end - table[2]) / 0.001 + 1), 1);
        temp_x = A * p_x;
        temp_y = A * p_y;
        for(int i = 0; i < n; i++){
            delta_temp_x(i, 0) = q_x(i, 0) - temp_x(static_cast<int>((t_end - table[2]) / n * i + table[2]), 0);
            delta_temp_y(i, 0) = q_y(i, 0) - temp_y(static_cast<int>((t_end - table[2]) / n * i + table[2]), 0);
        }
        Eigen::EigenSolver<Eigen::MatrixXd> eigenSolver(A.transpose() * A);
        //std::cout<<"values is: "<<eigenSolver.eigenvalues()<<std::endl;
        double max = 0;
        for(auto x : eigenSolver.eigenvalues()){
            if(max < x.real()){
                max = x.real();
            }
        }
        double miu = 1 / max;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                delta_x(i, 0) += delta_temp_x(j, 0) * A(static_cast<int>((t_end - table[2]) / n * j + table[2]), i);
                delta_y(i, 0) += delta_temp_y(j, 0) * A(static_cast<int>((t_end - table[2]) / n * j + table[2]), i);
            }
            delta_x(i, 0) *= miu;
            delta_y(i, 0) *= miu;
        }
        p_x = p_x + delta_x;
        p_y = p_y + delta_y;
    }
    Eigen::MatrixX<double> temp_x(static_cast<int>((t_end - table[2]) / 0.001 + 1), 1);
    Eigen::MatrixX<double> temp_y(static_cast<int>((t_end - table[2]) / 0.001 + 1), 1);
    temp_x = A * p_x;
    temp_y = A * p_y;
    for(int i = 0; i < n; i++){
        Circle(QPoint(p_x(i, 0), p_y(i, 0)), 5).draw(painter);
    }
    for(size_t i = 0; i < static_cast<size_t>((t_end - table[2]) / 0.001 + 1); i++){
        painter.drawPoint(static_cast<int>(temp_x(i, 0) + 0.5), static_cast<int>(temp_y(i, 0) + 0.5));
    }
}

*/

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
