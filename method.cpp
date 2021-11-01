#include "method.h"
#include <cmath>


double get_angle(int x1, int y1, int x2, int y2, int x3, int y3){
    double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
        if (theta > M_PI)
            theta -= 2 * M_PI;
        if (theta < -M_PI)
            theta += 2 * M_PI;
     // theta = abs(theta * 180.0 / M_PI);
     // std::cout<<"res: "<<theta<<std::endl;
     return theta;

}
