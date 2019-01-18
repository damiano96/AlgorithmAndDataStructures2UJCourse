#include <iostream>
#include "lab5.h"

class Vertex : public Object {
    public:
        typedef unsigned int Number;
        Vertex(Number);
        virtual operator Number() const;
        int CompareTo (Object const&) const;
        void Put(ostream&) const;
Number number;
    protected:
        
};

class Edge : public Object {
    protected:
        Vertex& v0;
        Vertex& v1;
    public:
        Edge(Vertex&, Vertex&);
        virtual Vertex& V0() const;
        virtual Vertex& V1() const;
        virtual Vertex& Mate(Vertex&); 

        int CompareTo (Object const&) const;
        void Put(ostream&) const;
};

class Graph {
    protected:
        unsigned int numberOfVertices;
        unsigned int numberOfEdges;
    public:
        virtual unsigned int NumberOfEdges () const;
        virtual unsigned int NumberOfVertices () const;
        virtual void AddVertex (Vertex&) = 0;
        virtual Vertex& SelectVertex (Vertex::Number) const = 0;
        virtual Vertex& operator [] (Vertex::Number) const;
        virtual void AddEdge (Edge&) = 0;
        virtual Edge& SelectEdge (Vertex::Number, Vertex::Number) = 0;
        virtual bool IsEdge (Vertex::Number, Vertex::Number) = 0;
        virtual bool IsConnected () = 0;
        virtual bool IsCyclic () const;
        
        virtual Iterator& Vertices () const = 0;
        virtual Iterator& Edges () const = 0;
        virtual Iterator& IncidentEdges (Vertex const&) const = 0;
       // virtual Iterator& EmanatingEdges (Vertex&);

        void DepthFirstTraversal(PrePostVisitor&, Vertex&);
        void DepthFirstTraversal(PrePostVisitor&, Vertex&, Array<bool>& visited);
};

class GraphAsMatrix : public virtual Graph {
    protected:
        Array<Vertex*> vertex;
        Array2D<Edge*> edge;
    
    public:
        GraphAsMatrix(unsigned int);
       
        void AddVertex (Vertex&);
        Vertex& SelectVertex (Vertex::Number) const;
        void AddEdge (Edge&);
        Edge& SelectEdge (Vertex::Number, Vertex::Number);
        bool IsEdge (Vertex::Number, Vertex::Number) ;
        bool IsConnected ();
        Iterator& IncidentEdges (Vertex const&) const;
        Iterator& EmanatingEdges (Vertex&);
        Iterator& Vertices () const;
        Iterator& Edges () const;

        void DepthFirstTraversal(PrePostVisitor&, Vertex&);
        void DepthFirstTraversal(PrePostVisitor&, Vertex&, Array<bool>& visited);
        
};

Vertex::Vertex(Number number) { this -> number = number; }

Vertex::operator Number() const { return number; }

int Vertex::CompareTo(Object const& object) const {}

void Vertex::Put(std::ostream& s) const { s << number << " "; }

/*Edge*/

Edge::Edge(Vertex& v0, Vertex& v1) : v0(v0), v1(v1) {
    this->v0 = v0;
    this->v1 = v1;
}

Vertex& Edge::V0() const { return v0; }

Vertex& Edge::V1() const { return v1; }

Vertex& Edge::Mate(Vertex& v) {
    if(&v==&v0) return v1;
    else if (&v == &v1) return v0;
    else throw invalid_argument("Blad !\n");
}

int Edge::CompareTo(Object const& object) const { }

void Edge::Put(std::ostream& s) const { s << V0() << "i " << V1() << "\t"; }


/*Graph*/
unsigned int Graph::NumberOfEdges() const { return numberOfEdges; }
unsigned int Graph::NumberOfVertices() const { return numberOfVertices; }
Vertex& Graph::operator [] (Vertex::Number num) const { SelectVertex(num); }

bool Graph::IsCyclic () const { return true; }


/*GraphAsMatrix*/
GraphAsMatrix::GraphAsMatrix(unsigned int a) : edge(a,a) , vertex(a) { 
     for (int i = 0; i < a; ++i) {
            for (int j = 0; j < a; ++j) {
                edge[i][j] = NULL;
            }
        }
        
}

void GraphAsMatrix::AddVertex (Vertex& v) {
    if(&v==NULL || v > vertex.Length()) throw std::invalid_argument("Blad dodawania");
    unsigned int n = NumberOfVertices();
    vertex[n] = &v;
    numberOfVertices++;
}

Vertex& GraphAsMatrix::SelectVertex (Vertex::Number number) const {
    if(vertex[number] != NULL) return *vertex[number];
    throw std::invalid_argument("Brak wierzchołka");
}

void GraphAsMatrix::AddEdge(Edge& e) {
    unsigned int A = e.Edge::V0();
    unsigned int B = e.Edge::V1();
    edge[A][B] = &e;
    edge[B][A] = &e; 
    numberOfEdges++;
    
}

Edge& GraphAsMatrix::SelectEdge (Vertex::Number n1, Vertex::Number n2) {
   if(edge[n1][n2] != NULL && edge[n2][n1] != NULL) return *edge[n1][n2];
   // else throw std::invalid_argument("Blad, nie ma krawedzi ");
}

bool GraphAsMatrix::IsEdge (Vertex::Number n1, Vertex::Number n2) {
    if(edge[n1][n2] != NULL && edge[n2][n1] != NULL) return true;
    else return false;
}


/*usless poki co*/
Iterator& GraphAsMatrix::Vertices () const {}
Iterator& GraphAsMatrix::Edges () const  {}
Iterator& GraphAsMatrix::IncidentEdges (Vertex const&) const {}



/* Sprawdzamy czy graf jest spojny, tzn. czy dla każdych dwóch jego wierzchołków istnieje ścieżka, które je ze sobą łączy.
Tworzymy licznik odwiedzanych wierzcholkow. 
Nastepnie uruchamiany DFS  od wierzcholka 0, przy kazdym odwiedzonym wierzcholku zwikesamy licznik
Jezwli liczba bedzie rowna liczbie wierzcholkow to graf jest connected*/

/******############# ZADANIE 5 ###########***********/
class CountingVisitor : public Visitor {
    private:
        unsigned int count;
    public:
        CountingVisitor() : count(0) {}
        unsigned int Count() const;
        void Visit(Object&);
};

void GraphAsMatrix::DepthFirstTraversal(PrePostVisitor& visitor, Vertex &start) {
    Array<bool> visited (numberOfVertices); //tworzymy tablice do odwiedzonych wierzchokow
    Vertex::Number v = 0;
    for(v; v<numberOfVertices; ++v) { visited[v] = false; } //wypelniamy ja falsami najpierw
    DepthFirstTraversal(visitor, const_cast<Vertex&> (start), visited); //wywolanie DFS
}

void GraphAsMatrix::DepthFirstTraversal(PrePostVisitor& visitor, Vertex& vertex2, Array<bool> &visited) {
    visitor.PreVisit(vertex2); //zaznaczyamy ze wstepnie dany wierzcholek odwiedzamy
    visited[Vertex::Number(vertex2)] = true; //tablice odwiedzin dajemy na true na danym wiercholku na ktory wchodzimy
    
    for(Iterator& p = EmanatingEdges(vertex2); !p.IsDone(); ++p) { //iteratorem idziemy po wszystkich wiercholkach
        Vertex& to = ((Edge&)(*p)).Mate(vertex2); //wpisujemy docelowy wiercholek(sasiad) do obiektu vertex
        if(!visited[Vertex::Number(to)]) { DepthFirstTraversal(visitor, to, visited); } //sprawdzamy czy wierzcholek docelowy nie zostal juz odwiedzony, jezeli nie to wywolujemy rekurencyjnie dfs 
                                                                                        //z nowym parametrem  
    }
}

bool GraphAsMatrix::IsConnected() {
    CountingVisitor countingVisitor; //nowy obiekt Coutingvisitor
    PreOrder preorder(countingVisitor); 
    DepthFirstTraversal(preorder, SelectVertex(0)); //wywolanie DFS od zerowego wierzcholka
    return countingVisitor.Count() == numberOfVertices; //return w zaleznosci czy liczba wierchokkow jest rowna liczbie odwiedzin
}

unsigned int CountingVisitor::Count() const { return count; }
void CountingVisitor::Visit(Object& object) { ++count; }


/*iteratory */

class EmantingEdgesIterator : public Iterator { //iterator krawedzi wychodzacych z danego wierzcholka
    GraphAsMatrix *owner;

    int row, col;
    void next();
    public:
        EmantingEdgesIterator(Vertex &v, GraphAsMatrix &owner) : owner(&owner), row(Vertex::Number(v)), col(-1) {
            next();
        }
        virtual bool IsDone() const;
        virtual Edge& operator*() const;
        virtual void operator++();
        virtual void Reset();
};

void EmantingEdgesIterator::next() {
    do {
        if(++col >= owner->NumberOfVertices()) { return; }
    } while (&owner->SelectEdge(row,col) == NULL); 
}

bool EmantingEdgesIterator::IsDone() const { return col >= owner->NumberOfVertices(); }
Edge& EmantingEdgesIterator::operator*() const { return (owner->SelectEdge(row, col) ); }
void EmantingEdgesIterator::operator++() { next(); }
void EmantingEdgesIterator::Reset(){}

Iterator& GraphAsMatrix::EmanatingEdges (Vertex &v) { 
    return *(new EmantingEdgesIterator(v, *this));
}



int main () {
    Vertex v1(2);
    Vertex v2(1);
    Vertex v3(0);
    Vertex v4(3);
    Vertex v5(4);
   
    Edge e1(v1, v2);
    Edge e2(v1, v3);
    Edge e3(v3, v4);
    Edge e4(v4, v1);

    GraphAsMatrix a(7);
    a.AddVertex(v1);
    a.AddVertex(v2);
    a.AddVertex(v3);
    a.AddVertex(v4);
    a.AddVertex(v5);

    a.AddEdge(e1);
    a.AddEdge(e2);
    a.AddEdge(e3);
    a.AddEdge(e4);
    

    if(a.IsConnected()) std::cout << "Is Connected" << std::endl;
    else std::cout<<"Isn't Connected" << std::endl;
    
}