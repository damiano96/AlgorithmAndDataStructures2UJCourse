#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

class Points {
    private:
        double x,y;

    public:
        Points(double,double);
        double getX() const;
        double getY() const;
        bool operator < (Points) const;
        bool operator!= (Points) const;
};
Points::Points(double x, double y) {
    this->x = x;
    this->y = y;
}
double Points::getX() const { return x; }
double Points::getY() const { return y; }

bool Points::operator<(Points b) const {
    if(y < b.getY()) return true;
    else if(y == b.getY()) return (x<b.getX());
    else return false;
}
bool Points::operator!=(Points b) const {
    if((y == b.getY()) && (x == b.getX())) return false;
    else return true;
}

double countAngle(Points a, Points b, Points c) { //funkcja do liczenia kat miedzy wketorami, uzywam tutaj funckji wbudowanej atan2
    double ax = a.getX();
    double bx = b.getX();
    double cx = c.getX();
    double ay = a.getY();
    double by = b.getY();
    double cy = c.getY();
    
    double atanA = atan2(ay - by, ax - bx);
    double atanC = atan2(cy - by, cx - bx);
    double diff = atanA- atanC;
    if (diff > M_PI) diff -= 2*M_PI;
    else if (diff < -M_PI) diff += 2*M_PI;

  // Convert to degrees
    diff *= 180 / M_PI;
    if(diff<0) diff = 360+diff;

    return diff;
}
int maxAngle(std::vector<Points> test, Points a, Points b) {  //metoda wyznajczajaca maksymalny kat 
    std::vector<double> angles;
    int returnIndex = 0;

    for(int i=0; i<test.size(); i++) angles.push_back(countAngle(a,b,test[i]));

    double maxAngle = angles[0];
    for(int i=0; i<test.size(); i++) {
        if(test[i] != b) {
            if(maxAngle <= angles[i]) {
                maxAngle = angles[i];
                returnIndex = i;
            }
        }
    }
    return returnIndex;
}   
bool isExist(std::vector<Points> input, std::vector<Points> point, int maxIndex) { //metoda do sprawdzenia czy dany punkt juz 
    bool test = false;
    for(int d=0; d<input.size(); d++) {
        if((input[d] != point[maxIndex]) || (point[maxIndex] != point[0])) test = true; 
        else test = false;
    }
    return test;
}
std::vector<Points> jarvisAlgorithm(std::vector<Points> point, int n) {
    std::vector<Points> result;
    int minimumPoint = 0;

    if(n<3) throw ("Not enough points");

    for(int i=1; i<n; i++) {  //szukamy najmniejszego punktu y
        if(point[i] < point[minimumPoint])  {
            minimumPoint = i;
        }
    }
    
    std::swap(point[0], point[minimumPoint]); //zamiana tak, aby najmniejszy punkt byl na poczatku
    result.push_back(point[0]); //wrzucenie go do wysciowego wektora
    
    Points p0(point[0].getX()-99, point[0].getY()); //stworzenie punktu p0, z najmnijesza wspolrzedna x
    int j = 0, z =0;
    
    int maxIndex = maxAngle(point, p0, point[z]);
    
    if(isExist(result, point, maxIndex))  result.push_back(point[maxIndex]);  //pierwsze sprawdzenie czy dany punkt znajduje sie juz w vectorze
   
    while(true) { 
        j = z;
        z = maxIndex;
        maxIndex = maxAngle(point, point[j], point[z]); //ustalamy punkt o najwyzszym kącie
        if(!(point[maxIndex] != point[0])) break; //sprawdzamy warunek zakonczenia dzialania algorytmu
        if(isExist(result, point, maxIndex))  result.push_back(point[maxIndex]);  
   } 
    return result;
}

void printResultPoints(std::vector<Points> result) {
    for(int i=0; i<result.size(); i++) std::cout << result[i].getX() << " " << result[i].getY() << std::endl;
}

int main () {   
 /*   Points p1(9,9);
    Points p2(3,8);
    Points p3(8,3);
    Points p4(2,2);
    Points p5(6,6);
    Points p6(4,4);
    Points p7(5,1);
    Points p8(3,6);
    Points p9(7,3);
    Points p10(8,7); 
    
    std::vector<Points> points = {p1,p2,p3,p4,p5,p6,p7,p8,p9,p10}; */

 std::vector<Points> points = {{0, 3}, {2, 2}, {1, 1}, {2, 5}, {3, 0}, {0, 0}, {3, 3}};
int n = points.size();
    std::vector<Points> resul;
    resul = jarvisAlgorithm(points, n);

 
    printResultPoints(resul);


}