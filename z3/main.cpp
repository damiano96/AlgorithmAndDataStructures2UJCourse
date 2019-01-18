#include <iostream>
#include "lab4.h"

class Vertex : public Object {
    public:
        typedef unsigned int Number;
        Vertex(Number);
        virtual operator Number() const;
        int CompareTo (Object const&) const;
        void Put(ostream&) const;

    protected:
        Number number;
};

class Edge : public Object {
    protected:
        Vertex& v0;
        Vertex& v1;
    public:
        Edge(Vertex&, Vertex&);
        virtual Vertex& V0() const;
        virtual Vertex& V1() const;
        virtual Vertex& Mate(Vertex const&) const; 

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
        virtual bool IsConnected () const;
        virtual bool IsCyclic () const;
        
        virtual Iterator& Vertices () const = 0;
        virtual Iterator& Edges () const = 0;
        virtual Iterator& IncidentEdges (Vertex const&) const = 0;
        virtual Iterator& EmanatingEdges (Vertex const&) const = 0;
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
        Iterator& IncidentEdges (Vertex const&) const;
        Iterator& EmanatingEdges (Vertex const&) const;
        Iterator& Vertices () const;
        Iterator& Edges () const;
};

Vertex::Vertex(Number number) {
    this -> number = number;
}

Vertex::operator Number() const { return number; }

int Vertex::CompareTo(Object const& object) const {}

void Vertex::Put(std::ostream& s) const {
    s << number << " ";
}

/*Edge*/

Edge::Edge(Vertex& v0, Vertex& v1) : v0(v0), v1(v1) {
    this->v0 = v0;
    this->v1 = v1;
}

Vertex& Edge::V0() const { return v0; }

Vertex& Edge::V1() const { return v1; }

Vertex& Edge::Mate(Vertex const& v) const {
    if(v==v0) return v1;
    else if (v == v1) return v0;
    else throw invalid_argument("Blad !\n");
}

int Edge::CompareTo(Object const& object) const {
}

void Edge::Put(std::ostream& s) const { s << V0() << "i " << V1() << "\t"; }


/*Graph*/
unsigned int Graph::NumberOfEdges() const { return numberOfEdges; }
unsigned int Graph::NumberOfVertices() const { return numberOfVertices; }
Vertex& Graph::operator [] (Vertex::Number num) const { SelectVertex(num); }

bool Graph::IsConnected () const { return true; }
bool Graph::IsCyclic () const { return true; }


/*GraphAsMatrix*/
GraphAsMatrix::GraphAsMatrix(unsigned int a) : edge(a,a) , vertex(a) {
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
   if(edge[n1][n2] != NULL && edge[n1][n2] != NULL) return *edge[n1][n2];
    else throw std::invalid_argument("Blad, nie ma krawedzi ");
}

bool GraphAsMatrix::IsEdge (Vertex::Number n1, Vertex::Number n2) {
    if(edge[n1][n2] != NULL && edge[n2][n1] != NULL) return true;
    else return false;
}


/*usless poki co*/
Iterator& GraphAsMatrix::Vertices () const {}
Iterator& GraphAsMatrix::Edges () const  {}
Iterator& GraphAsMatrix::IncidentEdges (Vertex const&) const {}
Iterator& GraphAsMatrix::EmanatingEdges (Vertex const&) const {}


int main () {
    int x,y;

    Vertex v1(2);
    Vertex v2(1);
    Vertex v3(4);
    Vertex v4(6);
    Vertex v5(3);
    Edge e1(v1, v2);
    Edge e2(v1, v3);
    Edge e3(v5, v3);
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

    std::cout << "* Lista wierzcholkow: ";
    v1.Put(std::cout);
    v2.Put(std::cout);
    v3.Put(std::cout);
    v4.Put(std::cout);
    v5.Put(std::cout);

    std::cout << "\n* Lista krawedzi miedzy wierzcholkami: ";
    e1.Put(std::cout);
    e2.Put(std::cout);
    e3.Put(std::cout);
    e4.Put(std::cout);

    x = 4;
    unsigned int i = a.SelectVertex(x);
    std::cout << "\n* Wierzcholek nr '" << x << "' ma wartosc " << i;

    x=6;
    y=2;
    if(a.IsEdge(x,y)) std::cout << "\n* Istnieje krawedz miedzy wierzcholkami: " << x << " i " << y << "\n";
    else std::cout << "\n* Nie ma krawedzi miedzy " << x << " , " << y << "\n";

    x = 1;
    y = 6;
    if(a.IsEdge(x,y)) std::cout << "* Istnieje krawedz miedzy wierzcholkami: " << x << " i " << y << "\n";
    else std::cout << "* Nie ma krawedzi miedzy " << x << " , " << y << "\n";

    x = 2;
    y = 6;
    if(a.IsEdge(x,y)) std::cout << "* Istnieje krawedz miedzy wierzcholkami: " << x << " i " << y << "\n";
    else std::cout << "* Nie ma krawedzi miedzy " << x << " , " << y << "\n";

}