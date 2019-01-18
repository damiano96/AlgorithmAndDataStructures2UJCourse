#include <iostream>

int main() {
    double det, xp, yp, xq, yq, xr, yr, one = 1;

    //punkt p
    xp = 1, yp = 2;
    //punkt q
    xq = 5, yq = 4;
    //punkt r 
    xr = 3, yr = 3;

    det = xp*yq*one+yp*one*xr+one*xq*yr-(yp*xq*one+xp*one*yr+one*yq*xr);

    std::cout << "Punkty: " << "P(" << xp << " , " << yp << "), " << "Q(" << xq << " , " << yq << "), " << "R(" << xr << " , " << yr << ")" << std::endl;

    if(det>0) {
        std::cout << "Punkt R, lezy po lewej stronie wektora P->Q";
    } else if (det<0) {
        std::cout << "Punkt R, lezy po prawej stronie wektora P->Q";
    } else {
        std::cout << "Punkty ss wspoliniowe";
    }

    std::cout << std::endl;
}