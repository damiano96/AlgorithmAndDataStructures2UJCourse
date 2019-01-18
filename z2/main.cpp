#include "lab3.h"
#include <iostream>

using namespace std;
class PriorityQueue : public virtual Container {
    virtual void Enqueue (Object&) = 0;
    virtual Object& FindMin() const = 0;
    virtual Object& DequeueMin () = 0;
};

class BinaryHeap : public PriorityQueue {
    private:
        Array<Object*> array;
    
    public:
        BinaryHeap(unsigned int);
        ~BinaryHeap();

        void Purge();
        void Put(ostream&) const;

        void Enqueue(Object&);
        Object& FindMin() const;
        Object& DequeueMin();

        int CompareTo(Object const&) const;
	    void Accept(Visitor &) const;
	
};

BinaryHeap::BinaryHeap(unsigned int z) : array(z, 1) {}
BinaryHeap::~BinaryHeap() {
    Purge();
}

void BinaryHeap::Purge() {
    if(count == 0) {
        cout << "Kolejka jest pusta\n";
        exit(0);
    }
    unsigned int i=1;
    unsigned int max = count;
    if(IsOwner()) {
        for(i; i<=max; ++i) {
           array[i] = NULL;
        }
    }
    count = 0;
    
}

void BinaryHeap::Put(ostream& s) const {
    if(count == 0) {
        cout << "Kolejka jest pusta\n";
        exit(0);
    }
    unsigned int max = count;
    for(int i=1; i<=max; i++) {
        s << *array[i] << " ";
    }
    s << endl;
}

void BinaryHeap::Enqueue(Object& obj) {
    if(count == array.Length()) {
        throw domain_error ("full");
    }
    ++count;
    unsigned int i = count;
    while(i>1 && *array[i/2] > obj) {
        array[i] = array[i/2];
        i /= 2;
    }
    array[i] = &obj;
}

Object& BinaryHeap::FindMin() const {
    if(count==0) {
        cout << "Kolejka jest pusta\n";
        exit(1);
    }
    return *array[1];
}

Object& BinaryHeap::DequeueMin() {
    Object& result = *array[1];
    Object& last = *array[count];
    --count;
    unsigned int i = 1;
    while(2*i < count +1) {
        unsigned int child = 2*i;
        if(child + 1 < count +1 && *array[child + 1] < *array[child]) {
            child += 1;
        }
        if(last <= *array[child]) {
            break;
        }
        array[i] = array[child];
        i=child;
    }
    array[i] = &last;
    return result;
}

int BinaryHeap::CompareTo(Object const& obj) const {}
void BinaryHeap::Accept(Visitor &vist) const {}

int main() {
    BinaryHeap bin(11);
    int Length, number;

    cout << "Ilosc liczb w kopcu: ";
    cin >> Length;

    Wrapper<unsigned int> liczby[Length];

    cout << "Wpisz liczby: " << endl; 
    
    for(int i=0; i<Length; i++) {
        cin >> number;
        liczby[i] = Wrapper<int> (number);
        bin.Enqueue(liczby[i]);
    } 

    
    cout << "\n\n*Zawarosc kopca:" << endl;
	bin.Put(std::cout);

    cout << "*Najmniejszy element: " << bin.FindMin() << endl;

	cout << "*Usuwam elementy:" << endl;
    for(int i=1; i<=Length; i++) {
        bin.DequeueMin();
        bin.Put(cout);
    }

    return 0;
} 