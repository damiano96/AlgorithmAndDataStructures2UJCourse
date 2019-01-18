#include <iostream>
#include <queue>
#include <list>
#include <iterator>
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
        //virtual Iterator& Edges () const = 0;
        //virtual Iterator& IncidentEdges (Vertex const&) const = 0;
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
        Iterator& IncidentEdges (Vertex&);
        Iterator& EmanatingEdges (Vertex&);
        Iterator& Vertices () const;
        Iterator& Edges ();

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
    //edge[B][A] = &e; 
    numberOfEdges++;

    
}

Edge& GraphAsMatrix::SelectEdge (Vertex::Number n1, Vertex::Number n2) {
   if(edge[n1][n2] != NULL) return *edge[n1][n2];
  // else throw std::invalid_argument("Blad, nie ma krawedzi ");
}

bool GraphAsMatrix::IsEdge (Vertex::Number n1, Vertex::Number n2) {
    if(edge[n1][n2] != NULL && edge[n2][n1] != NULL) return true;
    else return false;
}

/*usless poki co*/
Iterator& GraphAsMatrix::Vertices () const {}

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
    Array<bool> visited (numberOfVertices);
    Vertex::Number v = 0;
    for(v; v<numberOfVertices; ++v) { visited[v] = false; }
    DepthFirstTraversal(visitor, const_cast<Vertex&> (start), visited);
}

void GraphAsMatrix::DepthFirstTraversal(PrePostVisitor& visitor, Vertex& vertex2, Array<bool> &visited) {
    visitor.PreVisit(vertex2);
    visited[vertex2] = true;
    Iterator& p = EmanatingEdges(vertex2);
    while(!p.IsDone()) {
        Vertex& to = ((Edge&)(*p)).Mate(vertex2);
        if(!visited[Vertex::Number(to)]) { DepthFirstTraversal(visitor, to, visited); }
        ++p;
    }
    delete &p;
    visitor.PostVisit(vertex2); 
}


bool GraphAsMatrix::IsConnected() {
    CountingVisitor countingVisitor;
    PreOrder preorder(countingVisitor);
    DepthFirstTraversal(preorder, SelectVertex(0));
    
    return countingVisitor.Count() != numberOfVertices;
}

unsigned int CountingVisitor::Count() const { return count; }
void CountingVisitor::Visit(Object& object) { ++count; }

/*iteratory */
class EmantingEdgesIterator : public Iterator {
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

Iterator& GraphAsMatrix::EmanatingEdges (Vertex &v) { return *(new EmantingEdgesIterator(v, *this));}

void EmantingEdgesIterator::next() {
   do {
        if(++col >= owner->NumberOfVertices()) { return; }
    } while (&owner->SelectEdge(row,col) == NULL); 
}

bool EmantingEdgesIterator::IsDone() const { return col >= owner->NumberOfVertices(); }
Edge & EmantingEdgesIterator::operator*() const { return (owner->SelectEdge(row, col) ); }
void EmantingEdgesIterator::operator++() { next(); }
void EmantingEdgesIterator::Reset(){}

class IncEdgesIterator : public Iterator {
    GraphAsMatrix *owner;
    int row, col;
    void next();

    public: 
        IncEdgesIterator (Vertex &v, GraphAsMatrix &owner) : owner(&owner), row(-1), col(Vertex::Number(v)) {
            next();
        }
        virtual bool IsDone() const;
        virtual Edge& operator*() const;
        virtual void operator++();
        virtual void Reset();
};

Iterator& GraphAsMatrix::IncidentEdges(Vertex &v) { return *(new IncEdgesIterator(v, *this)); }

void IncEdgesIterator::next() {
    do {
        if(++row >= owner->NumberOfVertices()) {
             return;
        }
    } while (&owner->SelectEdge(row,col) == NULL);
}
bool IncEdgesIterator::IsDone() const { return row >= owner->NumberOfVertices(); }
Edge & IncEdgesIterator::operator*() const { return (owner->SelectEdge(row, col) ); }
void IncEdgesIterator::operator++() { next(); }
void IncEdgesIterator::Reset(){}

class AllEdgesIterator : public Iterator {
    GraphAsMatrix *owner;
    int row, col;
    void next();

    public: 
        AllEdgesIterator (GraphAsMatrix &owner) : owner(&owner), row(0), col(-1) {
            next();
        }
        virtual bool IsDone() const;
        virtual Edge& operator*() const;
        virtual void operator++();
        virtual void Reset();
};

Iterator& GraphAsMatrix::Edges() { return *(new AllEdgesIterator(*this)); }

void AllEdgesIterator::next() {
    do {
        if(++col >= owner->NumberOfVertices()) {
            col = 0; 
            if(++row >= owner->NumberOfVertices()) return;
        }
    } while (&owner->SelectEdge(row,col) == NULL);
}
bool AllEdgesIterator::IsDone() const { return row >= owner->NumberOfVertices(); }
Edge & AllEdgesIterator::operator*() const { return (owner->SelectEdge(row, col) ); }
void AllEdgesIterator::operator++() { next(); }
void AllEdgesIterator::Reset(){}


/*zadanie 6*/
/* Badanie spojnosci grafu */

class Digraph : public Graph {
    public:
        virtual bool IsCyclic();
 };

class DigraphAsMatrix : public GraphAsMatrix {
    public:
        DigraphAsMatrix(unsigned int x) : GraphAsMatrix(x) {} 

        void TopologicalOrderTraversal(Visitor&);
        bool IsCyclic();
};

bool DigraphAsMatrix::IsCyclic() {
    CountingVisitor visitor;
    
    TopologicalOrderTraversal(visitor);
    return visitor.Count() != numberOfVertices;
}

void DigraphAsMatrix::TopologicalOrderTraversal(Visitor &v) {
    Array<unsigned int> inDegree (numberOfVertices); //tworzymy nowa tablice int 
    
    list<Vertex> vertices; //tworzymy liste do przechowywania wierzchokkow
    for(int i = 0; i < numberOfVertices; i++) {
        inDegree[i] = 0; //inicjiujemy ja zerami
        vertices.push_back(SelectVertex(i)); //wrzucam na liste kolejne wierzcholki
        for(Iterator& it = IncidentEdges(SelectVertex(i)); !it.IsDone(); ++it) //iterator po krawedziach wchodzacych do wierzcholka
            inDegree[i]++; //dodatkowo zwiekszamy tablice wystepujacymi krawedziami
    }
    bool test; //zmienna boolowska testujaca 
    std::cout << "Sortowanie topologiczne: ";
    do {
        test = false; //ustawiamy ja na falsz
        for (auto i = vertices.begin(); i != vertices.end();) { //przechodzimy przez liste
            if (inDegree[Vertex::Number(*i)] == 0) {  //jezeli liczba krawedzi do wierzcholka rowna 0
                test = true;
                for (Iterator &it = EmanatingEdges(SelectVertex(Vertex::Number(*i))); !it.IsDone(); ++it) { //iterujemy po krawedziach
                    inDegree[Vertex::Number(dynamic_cast<Edge&>(*it).V1())]--; //zmniejszamy inDegree dla wierzchilka sasiada
                }
                std::cout << Vertex::Number(*i) << " ";
                v.Visit(*i); //wpisujemy sie ze bylismy w danym wierzcholku
                auto temp = i++; //usuwamy z listy wierzcholkow wierzcholek nastepny
                vertices.erase(temp);
            }
            else //w przeciwnym razie przechodzimy na nastepny element listy
                i++; 
        }
    } while (test);
    std::cout << std::endl;
}
int main() {
    Vertex v1(0);
    Vertex v2(1);
    Vertex v3(2);
    Vertex v4(3);
    Vertex v5(4);
    Vertex v6(5);


    DigraphAsMatrix dG(9);

    dG.AddVertex(v1);
    dG.AddVertex(v2);
    dG.AddVertex(v3);
    dG.AddVertex(v4);
    dG.AddVertex(v5);
    dG.AddVertex(v6);


    Edge ed1(v1,v2);
    dG.AddEdge(ed1);

    Edge ed2(v2,v3);
    dG.AddEdge(ed2);

    Edge ed3(v2,v5);
    dG.AddEdge(ed3);

    Edge ed4(v3,v4);
    dG.AddEdge(ed4);

    Edge ed5(v5,v6);
    dG.AddEdge(ed5);

    Edge ed6(v6, v5);
    dG.AddEdge(ed6);

    
    
    std::cout<<"IsCyclic: "<<dG.IsCyclic()<<std::endl;
 
}