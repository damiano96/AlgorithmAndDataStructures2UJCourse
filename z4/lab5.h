//
// This file contains the C++ code from 
// "Data Structures and Algorithms
// with Object-Oriented Design Patterns in C++"
// by Bruno R. Preiss.
//
// Copyright (c) 1998 by Bruno R. Preiss, P.Eng. All rights reserved.
//
// 
//

#include <typeinfo>
#include <stdexcept>	
using namespace std;


template <class T>
class Array
{
protected:
  T* data;
  unsigned int base;
  unsigned int length;
public:
  Array ();
  Array (unsigned int, unsigned int = 0);
  ~Array ();
  Array (Array const&);
  Array& operator = (Array const&);
  T const& operator [] (unsigned int) const;
  T& operator [] (unsigned int);
  T const* Data () const;
  unsigned int Base () const;
  unsigned int Length () const;
  void SetBase (unsigned int);
  void SetLength (unsigned int);  
};

template <class T> Array<T>::Array () : data (new T [0]), base (0),length (0){  
}

template <class T> Array<T>::Array (unsigned int n, unsigned int m) :data (new T [n]), base (m),length (n){  
}

template <class T> Array<T>::Array (Array<T> const& array) :data (new T [array.length]), base (array.base),length (array.length){
  for (unsigned int i = 0; i < length; ++i)
    data [i] = array.data [i];
}

template <class T> Array<T>::~Array (){
  delete [] data;  
}

template <class T> T const* Array<T>::Data () const{
  return data;  
}
template <class T> unsigned int Array<T>::Base () const{
   return base;   
}
template <class T> unsigned int Array<T>::Length () const{
   return length;   
}

template <class T> T const& Array<T>::operator [] (unsigned int position) const{
  unsigned int const offset = position - base;
  if (offset >= length)
    throw std::out_of_range ("invalid position");
  return data [offset];  
}

template <class T> T& Array<T>::operator [] (unsigned int position){
  unsigned int const offset = position - base;
  if (offset >= length)
    throw std::out_of_range ("invalid position");
  return data [offset];
}
 

template <class T>
class Array2D
{
protected:
    unsigned int numberOfRows;
    unsigned int numberOfColumns;
    Array<T> array;
public:
    class Row
    {
	Array2D& array2D;
	unsigned int const row;
    public:
	Row (Array2D& _array2D, unsigned int _row) :
	    array2D (_array2D), row (_row) {}
	T& operator [] (unsigned int column) const
	    { return array2D.Select (row, column); }
    };

    Array2D (unsigned int, unsigned int);
    T& Select (unsigned int, unsigned int);
//    Row operator [] (unsigned int);
    Row operator [] (unsigned int row){
      return Row (*this, row);        
    }
    
};

 
template <class T>
Array2D<T>::Array2D (unsigned int m, unsigned int n) :
    numberOfRows (m),
    numberOfColumns (n),
    array (m * n)
    {}

template <class T>
T& Array2D<T>::Select (unsigned int i, unsigned int j)
{
    if (i >= numberOfRows)
	throw out_of_range ("invalid row");
    if (j >= numberOfColumns)
	throw out_of_range ("invalid column");
    return array [i * numberOfColumns + j];
}

/*
template <class T>
Array2D<T>::Row Array2D<T>::operator [] (unsigned int row)
{
  return Row (*this, row);  
}


 
/* ============================================================ */
class Object
{
protected:
  virtual int CompareTo (Object const&) const = 0;
public:
  virtual ~Object ();
  virtual bool IsNull () const;
  virtual int Compare (Object const&) const;
// virtual HashValue Hash () const = 0;
  virtual void Put (ostream&) const = 0;
};

inline bool operator == (Object const& left, Object const& right)
{ return left.Compare (right) == 0; }
inline bool operator != (Object const& left, Object const& right)
{ return left.Compare (right) != 0; }
inline bool operator <= (Object const& left, Object const& right)
{ return left.Compare (right) <= 0; }
inline bool operator < (Object const& left, Object const& right)
{ return left.Compare (right) < 0; }
inline bool operator >= (Object const& left, Object const& right)
{ return left.Compare (right) >= 0; }
inline bool operator > (Object const& left, Object const& right)
{ return left.Compare (right) > 0; }
inline ostream& operator << (ostream& s, Object const& object)
{ object.Put (s); return s; }


Object::~Object ()
{}
bool Object::IsNull () const
{ return false; }

int Object::Compare (Object const& object) const
{
if (typeid (*this) == typeid (object))
  return CompareTo (object);
else if (typeid (*this).before (typeid (object)))
  return -1;
else
  return 1;
}

/* ============================================================ */

/* ============================================================ */
class NullObject : public Object
{
static NullObject instance;
  NullObject ();
protected:
  int CompareTo (Object const&) const;
public:
  bool IsNull () const;
  //HashValue Hash () const;
  void Put (ostream& s) const;
  static NullObject& Instance ();
};

NullObject NullObject::instance;
NullObject::NullObject(){}
bool NullObject::IsNull () const
{ return true; }
int NullObject::CompareTo (Object const&) const
{ return 0; } 
// HashValue NullObject::Hash () const
// { return 0; }
void NullObject::Put (ostream& s) const
{ s << "NullObject"; }
NullObject& NullObject::Instance ()
{ return instance; }
/* ============================================================ */

/* ============================================================ */
template <class T>
class Wrapper : public Object
{
protected:
  T datum;
  int CompareTo (Object const&) const;
public:
  Wrapper ();
  Wrapper (T const&);
  Wrapper& operator = (T const&);
operator T const& () const;
// HashValue Hash () const;
  void Put (ostream&) const;
};

template <class T>
Wrapper<T>::Wrapper () :
datum ()
{}
template <class T>
Wrapper<T>::Wrapper (T const& d) :
datum (d)
{}
template <class T>
Wrapper<T>& Wrapper<T>::operator = (T const& d)
{
datum = d;
return *this;
}
template <class T>
Wrapper<T>::operator T const& () const
{ return datum; }

int Compare (int a,int b) {
   if(a<b) return -1;
   if(a>b) return 1;
   return 0;  
}


template <class T>
int Wrapper<T>::CompareTo (Object const& obj) const
{
  Wrapper<T> const& arg =
  dynamic_cast<Wrapper<T> const&> (obj);
  return::Compare (datum, arg.datum);
}
template <class T>
void Wrapper<T>::Put (ostream& s) const
{ s << datum; }

typedef Wrapper<int> Int;
typedef Wrapper<unsigned int> UInt;
typedef Wrapper<char> Char;
typedef Wrapper<double> Double;
typedef Wrapper<std::string> String;

/* ============================================================ */

/* ============================================================ */
class Ownership
{
bool isOwner;
protected:
Ownership () : isOwner (true)
	{}
Ownership (Ownership& arg) : isOwner (arg.isOwner)
	{ arg.isOwner = false; }
public:
void AssertOwnership ()
	{ isOwner = true; }
void RescindOwnership ()
	{ isOwner = false; }
bool IsOwner () const
	{ return isOwner; }
};
/* ============================================================ */


/* ============================================================ */
class Visitor
{
public:
virtual void Visit (Object&) = 0;
virtual bool IsDone () const
	{ return false; }
};

/* ============================================================ */

/* ============================================================ */
class PuttingVisitor : public Visitor
{
  ostream& stream;
  bool comma;
public:
  PuttingVisitor (ostream& s) : stream (s), comma (false)	{}
  void Visit (Object& object)
  {
  if (comma)
    stream << ", "; 
    stream << object;
    comma = true;
  }
};
/* ============================================================ */

class PrePostVisitor : public Visitor
{
public:
    virtual void PreVisit (Object&) {}
    virtual void Visit (Object&) {}
    virtual void PostVisit (Object&) {}
};

/* ============================================================ */

class PreOrder : public PrePostVisitor
{
    Visitor& visitor;
public:
    PreOrder (Visitor& v) : visitor (v)
	{}
    void PreVisit (Object& object)
	{ visitor.Visit (object); }
};

/* ============================================================ */

class InOrder : public PrePostVisitor
{
    Visitor& visitor;
public:
    InOrder (Visitor& v) : visitor (v)
	{}
    void Visit (Object& object)
	{ visitor.Visit (object); }
};

/* ============================================================ */

class PostOrder : public PrePostVisitor
{
    Visitor& visitor;
public:
    PostOrder (Visitor& v) : visitor (v)
	{}
    void PostVisit (Object& object)
	{ visitor.Visit (object); }
};

/* ============================================================ */

class Iterator
{
public:
virtual ~Iterator (){ };
virtual void Reset () = 0;
virtual bool IsDone () const = 0;
virtual Object& operator * () const = 0;
virtual void operator ++ () = 0;
};
/* ============================================================ */
/*
/* ============================================================ */
class NullIterator : public Iterator
{
public:
NullIterator ();
~NullIterator () { }
void Reset ();
bool IsDone () const;
Object& operator * () const;
void operator ++ ();
};

NullIterator::NullIterator () {}

void NullIterator::Reset (){}

bool NullIterator::IsDone () const
{ return true; }

Object& NullIterator::operator * () const
{ return NullObject::Instance (); }

void NullIterator::operator ++ ()
{}

/* ============================================================ */

/* ============================================================ */
class Container : public virtual Object, public virtual Ownership
{
protected:
  unsigned int count;
  Container ();
public:
virtual unsigned int Count () const;
virtual bool IsEmpty () const;
virtual bool IsFull () const;
// virtual HashValue Hash () const;
virtual void Put (ostream&) const;
// virtual Iterator& NewIterator () const;
virtual void Purge () = 0;
virtual void Accept (Visitor&) const = 0;
Iterator& NewIterator () const{
  return *new NullIterator ();  
}
};

Container::Container () :count (0)
{}

unsigned int Container::Count () const
{ return count; }

bool Container::IsEmpty () const
{ return Count () == 0; }

bool Container::IsFull () const
{ return false; }

void Container::Put (ostream& s) const
{
PuttingVisitor visitor (s);
  s << typeid (*this).name () << " {";
  Accept (visitor);
  s << "}";
}

/* ============================================================ */


/* ============================================================ */
class SearchableContainer : public virtual Container
{
public:
  virtual bool IsMember (Object const&) const = 0;
  virtual void Insert (Object&) = 0;
  virtual void Withdraw (Object&) = 0;
  virtual Object& Find (Object const&) const = 0;
};
/* ============================================================ */
