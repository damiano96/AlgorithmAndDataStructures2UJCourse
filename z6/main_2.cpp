#include <iostream>

double countDeterminant(double* x, double* y, double* z) { //liczenie wyznacznika maceirzy
    double a1 = z[0] - x[0];
    double b1 = z[1] - x[1];

    double a2 = y[0] - x[0];
    double b2 = y[1] - x[1];

    return a1*b2 - a2*b1;
}

bool checkOnLineSegment(double* x, double* y, double* z) { //sprawdzenie wspolliniowosci punktow
    return std::min(x[0], y[0]) <= z[0] <= std::max(x[0], y[0]) && std::min(x[1], y[1]) <= z[1] <= std::max(x[1], y[1]);
}

bool checkFinally(double* p1, double* p2, double* p3, double* p4) {
    double d1 = countDeterminant(p3,p4,p1);
    double d2 = countDeterminant(p3,p4,p2);
    double d3 = countDeterminant(p1,p2,p3);
    double d4 = countDeterminant(p1,p2,p4);
    
    if(d1*d2 < 0 && d3*d4 < 0) return true;
    else {
        if((d1 == 0) && checkOnLineSegment(p3,p4,p1)) return true;
        else if((d2 == 0) && checkOnLineSegment(p3,p4,p2)) return true;
        else if((d3 == 0) && checkOnLineSegment(p1,p2,p3)) return true;
        else if((d4 == 0) && checkOnLineSegment(p1,p2,p4)) return true;

        else return false;
    }
}

int main() {
    double p1[2], p2[2], p3[2], p4[2];

    p1[0] = 1, p1[1] = 1;
    p2[0] = 3, p2[1] = 1;
    p3[0] = 2, p3[1] = 3;
    p4[0] = 2, p4[1] = 1;


    
    std::cout << checkFinally(p1,p2,p3,p4);
}