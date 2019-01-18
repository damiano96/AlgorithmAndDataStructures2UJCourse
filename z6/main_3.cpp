#include <iostream>
#include <stack>
#include <algorithm>

class Points {
    private:
        double x, y;
    
    public:
        Points() { }
        Points(double, double);
        double getX();
        double getY();

        double squareDistanceBetweenPoints(Points, Points);
        int clockOrientation(Points, Points);
        bool operator < (Points) const;  //sluzy do znalezienia punktu z najmneijsza y'kowa
};

Points pivot; //punkt odniesienia do pozniejszego sprawdzania

Points::Points(double x, double y) {
    this->x = x;
    this->y = y;
}

bool Points::operator < (Points b) const { 
    if(y < b.getY()) return true;  //porownanie wspolrzednej y a nastepnie wspolrzednej x
    else if(y==b.getY()) return (x < b.getX());
    else return false;
}

double Points::getX() { return x; }
double Points::getY() { return y; }

Points nexToTop(std::stack<Points> &stos) { //meotda do znalezienia elementu pod wierzcholkiem stosu
    Points temp = stos.top();
    stos.pop();
    Points to = stos.top();
    stos.push(temp);

    return to;
}
double Points::squareDistanceBetweenPoints(Points a, Points b) { //zwraca kwadrat dystansu pomiedzy punktami
    double ax = a.getX();
    double bx = b.getX();
    double ay = a.getY();
    double by = b.getY();

    return (ax-bx)*(ax-bx) + (ay-by)*(ay-by);
}

int Points::clockOrientation(Points b, Points c) { //odnazlenie orientacji trzech punktow
    double bx = b.getX();
    double cx = c.getX();
    double by = b.getY();
    double cy = c.getY();

    double orientation = bx*cy + cx*y + x*by - x*cy - bx*y - cx*by; //liczymy wyznacznik

    if(orientation == 0) return 0;      //wspoliniowowsc 
    else if(orientation > 0) return 2; //przeciwnie do wskazowek zegara
    else return 1; //
}

int polarAngle(const void *vp1, const void *vp2) {  //komparator, zwraca odpowiednio -1,1,2 w zaleznosci od wyniku porowania
    auto *a = (Points *) vp1;                      //uzywana jest przez funkcje biblioteczna qsort do sortowania tablicy punktow z odniesieniem do pierwszego punktu, 
    auto *b = (Points *) vp2;                      //dzieki niej qsort wie na na jakiej podstawie znajduje wiekszy/mniejszy lemen

    int order = pivot.clockOrientation(*a, *b);
    if(order == 0) {
        if(pivot.squareDistanceBetweenPoints(pivot, *b) >= pivot.squareDistanceBetweenPoints(pivot, *a)) return -1;
        else return 1;
    } else {
        if(order == 2) return -1;
        else return 1;
    }
}
std::stack<Points> GrahamScan(Points *points, int n) {
   //szukamy punktu z najmniejszym y
    int min = 0;
    for(int i=1; i<n; i++) {
        if(points[i] < points[min]) min = i; //wykorzystuje przeciazenie opratora do porownania punktow
    } 
    std::swap(points[0], points[min]); //usatwiamy punkt z najmnijeszym y na poczatek tablicy punktow
    pivot = points[0];
    std::qsort(&points[1], (n-1), sizeof(Points), polarAngle); //sortujemy n-1 punktow z odniesieinem do pierwszego punktu, punkt p1 bedzie przed punktem p2 po posortowaniu wtedy gdt
                                                                //ma wieksza wspolrzedna polarna niz p1

    int m=1;
    for(int i=1; i<n; i++) { //sprawdzamy czy dwa lub wiecej punktow ma ten sam kąt, ejzeli tak to usuwamy wszystkie punkty z wyjatkiem tego najdalej od pivota
        while(i<n-1 && pivot.clockOrientation(points[i], points[i+1]) == 0) i++;
        points[m++] = points[i];
    }
    std::stack <Points> result;
    if(n<3) return result; //jezeli w modyfikowanej tablicy jest mniej niz 3 punkty to GrahamAlg sie nie wykona

    result.push(points[0]);
    result.push(points[1]);
    result.push(points[2]);

    for(int i=3; i<m; ++i) {
        while(nexToTop(result).clockOrientation(result.top(), points[i]) != 2) result.pop(); //dopoki punkt pi jest na prawo wektora top nextToTop
        result.push(points[i]);
    }
    return result;
}

void printResultPoints(std::stack<Points> result) {
    while(!result.empty()){
        Points temp = result.top();
        std::cout << temp.getX() << " , " << temp.getY() << std::endl;
        result.pop();
    }
}

int main() {
  /*  Points p1(9,9);
    Points p2(3,8);
    Points p3(8,3);
    Points p4(2,2);
    Points p5(6,6);
    Points p6(4,4);
    Points p7(5,1);
    Points p8(3,6);
    Points p9(7,3);
    Points p10(8,7);*/
    
    Points points[] = {{0, 3}, {2, 2}, {1, 1}, {2, 5}, {3, 0}, {0, 0}, {3, 3}};

/*
     Point points[] = {{0, 3}, {1, 1}, {2, 2}, {4, 4},
                      {0, 0}, {1, 2}, {3, 1}, {3, 3}};*/
    int n = sizeof(points)/sizeof(points[0]);
    
    printResultPoints(GrahamScan(points, n));
    
    return 0;
}
