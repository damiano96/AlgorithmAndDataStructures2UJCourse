#include <iostream>
#include <cstdlib>
#include "lab2.h"

class Set : public virtual SearchableContainer {
	protected:
		unsigned int universeSize;
	
	public:
		Set(unsigned int);
		typedef Wrapper<unsigned int> Element;
};

class SetAsArray : public Set {
	private:
		Array<bool> array;
	
	public:
		SetAsArray(unsigned int);
		SetAsArray(SetAsArray&);

		void Insert(Object&);
		bool IsMember(Object const&) const;
		void Withdraw(Object&);

        void Insert(unsigned int);
		bool IsMember(unsigned int) const;
		void Withdraw(unsigned int);

        bool sizeOfUniverseSize(SetAsArray const&, SetAsArray const&);

		friend SetAsArray operator+ (SetAsArray const&, SetAsArray const&);
		friend SetAsArray operator* (SetAsArray const&, SetAsArray const&);
		friend SetAsArray operator- (SetAsArray const&, SetAsArray const&);

		void Put(std::ostream&) const;

        //meotdy nieuzywane
		int CompareTo(Object const&) const;
		void Purge();
		void Accept(Visitor&) const;
		Object& Find(Object const&) const;
};  

Set::Set(unsigned int n) {
    universeSize = n;
}

SetAsArray::SetAsArray(unsigned int x) : Set(x), array(x) {
    for(int i=0; i<universeSize; i++) {
        array[i] = false;
    }
}

void SetAsArray::Insert (Object& object) {
    unsigned int const i = dynamic_cast<Element&> (object);
    array[i] = true;
}
bool SetAsArray::IsMember(Object const& object) const {
    unsigned int const i = dynamic_cast<Element const&> (object);
    return array[i];    
}
void SetAsArray::Withdraw(Object& object) {
    unsigned int const i = dynamic_cast<Element&> (object);
    array[i] = false;
}

void SetAsArray::Insert (unsigned int n) {
    array[n] = true;
}
bool SetAsArray::IsMember(unsigned int n) const {
    return array[n];    
}
void SetAsArray::Withdraw(unsigned int n) {
    array[n] = false;
}

bool sizeOfUniverseSize(unsigned int a, unsigned int b) {
    if(a != b) {
        cout << "Wartosci zbiorow nie sa rowne\n\n";
        return false;
    } else {
        return true;
    }
}

SetAsArray operator+ (SetAsArray const& a, SetAsArray const& b) {
    if(!sizeOfUniverseSize(a.universeSize, b.universeSize)) {
        exit(1);
    }
    SetAsArray result (a.universeSize);

    for(int i=0; i<a.universeSize; i++) {
        result.array[i] = a.array[i] || b.array[i];
    }
    return result;
}

SetAsArray operator- (SetAsArray const& a, SetAsArray const& b) {
    if(!sizeOfUniverseSize(a.universeSize, b.universeSize)) {
        exit(1);
    }
    SetAsArray result (a.universeSize);

    for(int i=0; i<a.universeSize; i++) {
        result.array[i] = a.array[i] && !b.array[i];
    }
    return result;
}

SetAsArray operator* (SetAsArray const& a, SetAsArray const& b) {
    if(!sizeOfUniverseSize(a.universeSize, b.universeSize)) {
        exit(1);
    }
    SetAsArray result (a.universeSize);

    for(int i=0; i<a.universeSize; i++) {
        result.array[i] = a.array[i] && b.array[i];
    }
    return result;
}

void SetAsArray::Put(std::ostream& s) const {
    for(int i=0; i<universeSize; i++) {
        if(array[i]) {
            std::cout << i;
        }
    }
}

void SetAsArray::Accept(Visitor &v) const { }
void SetAsArray::Purge() {}
int SetAsArray::CompareTo(Object const& obj) const {}
Object& SetAsArray::Find(Object const& obj) const {}


int main () {

    SetAsArray set1(6);
    SetAsArray set2(10);
  
    for(int i=0; i<10; i++) {
        set1.Insert(i);
    }
    for(int i=0; i<10; i+=2) {
        set2.Insert(i);
    }

    cout << "Zbior A: \t{ ";
    set1.Put(std::cout);
    cout << " }";

    cout << "\nZbior B: \t{ ";
    set2.Put(std::cout);
    cout << " }";

    cout << "\nSuma: \t\t{ ";
    (set1+set2).Put(std::cout);
    cout << " }";

    cout << "\nRoznica: \t{ ";
    (set1-set2).Put(std::cout);
    cout << " }";
    
    cout << "\nPrzekroj: \t{ ";
    (set1*set2).Put(std::cout);
    cout << " }\n";

    cout << "Czy 2 jest elementem zbioru?: ";
    if(set1.IsMember(9)) cout << "tak"; 
    else cout << "nie";

    cout << "\nUsuniecie elemntu 4 { ";
    set2.Withdraw(4);
    set2.Put(std::cout);
    cout << " }\n";
}
