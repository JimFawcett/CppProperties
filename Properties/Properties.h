#pragma once
/////////////////////////////////////////////////////////////////////
// Properties.h - provides getter and setter methods               //
// ver 1.3                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single Property<T, E, C> class that provides
*  getter and setter methods and backing store T innerVar, using the property 
*  name.
*  - Here's an example:
*      Property<int> intProp;
*      intProp(42)    Sets its internal store to 42
*      intProp()      returns the internal store value
*  - So one declaration creates two methods with simple syntax, using
*    the property's instance name. 
*  - The getter and setter methods call constraint methods C::in and C::out
*    to provide logic that constrains getting and setting values.
*  - The property derives from an extension class E that can be configured
*    to supply methods you expect to be able to call on a T type.
*  - Property also provides the method ref() which returns a reference
*    to the underlying stored value.  That lets us call member functions
*    directly on the stored type.  However, that lets users bypass
*    getter and setter logic, so use it with caution.
*
*  Required Files:
*  ---------------
*  Properties.h
*
*  Maintenance History:
*  --------------------
*  ver 1.3 : 02 Jul 2019
*  - demonstrated that template specializations of Ext and Constraint
*    work as expected
*  ver 1.2 : 27 Jun 2019
*  - added Ext and Constraint classes
*  ver 1.1 : 30 Jan 2019
*  - added ref()
*  ver 1.0 : 11 Oct 2018
*  - first release
*/

#include <string>
#include <vector>
#include <stdexcept>

namespace Utilities
{
  ///////////////////////////////////////////////////////////////////
  // Ext provides a custom extension interface for properties
  // - You can define any methods that are syntactically correct
  //   for the type T
  // - Example: push_back when T is an STL container with method push_back 
  //
  // Ext is an externsion of Property<T> interface to include some or all
  // of T's methods.  That works because Property<T> derives from E<T>.

  template <typename T>
  struct Ext {
    Ext() : innerVar(T()) {}
    Ext(const T& t) : innerVar(t) {}
    virtual ~Ext() {}
    std::string typeIs() const
    {
      return typeid(innerVar).name();
    }
    size_t sizeIs() const
    {
      return sizeof(innerVar);
    }
    T innerVar;
  };

  /////////////////////////////////////////////////////////////////////
  // Template specialization of Ext<T> for std::string
  // - supports indexing and concatenation

  template <>
  struct Ext<std::string> {
    Ext() : innerVar(std::string()) {}
    Ext(const std::string& t) : innerVar(t) {}
    virtual ~Ext() {}
    void operator+=(const std::string& t)  // no chaining  because shouldn't return reference
    {
      innerVar += t;
    }
    char& operator[](size_t n)
    {
      if (n < 0 || n >= innerVar.size())
        throw std::out_of_range("index error in string property");
      return innerVar[n];
    }
    char operator[](size_t n) const
    {
      if (n < 0 || n >= innerVar.size())
        throw std::out_of_range("index error in string property");
      return innerVar[n];
    }
    std::string innerVar;
  };

  /////////////////////////////////////////////////////////////////////
  // Template specialization of Ext<T> for std::vector<T>
  // - supports indexing, iteration, insertion, and push_back

  template <typename T>
  struct Ext<std::vector<T>> {
    Ext() : innerVar(std::vector<T>()) {}
    Ext(const std::vector<T>& t) : innerVar(t) {}
    virtual ~Ext() {}

    using iterator = typename std::vector<T>::iterator;
    
    iterator begin() {
      return innerVar.begin();
    }
    iterator end() {
      return innerVar.end();
    }
    T pop_back()
    {
      return innerVar.pop_back();
    }
    void push_back(const T& t)
    {
      innerVar.push_back(t);
    }
    iterator insert(iterator iter, const T& t)
    {
      return innerVar.insert(iter, t);
    }
    T& operator[](size_t n)
    {
      if (n < 0 || n >= innerVar.size())
        throw std::out_of_range("index error in vector property");
      return innerVar[n];
    }
    T operator[](size_t n) const
    {
      if (n < 0 || n >= innerVar.size())
        throw std::out_of_range("index error in vector property");
      return innerVar[n];
    }
    std::vector<T> innerVar;
  };

  /////////////////////////////////////////////////////////////////////
  //// Constraint provides inner methods for the property's
  //// getter and setter methods.
  //// - That allows you to add custom logic for the built in
  ////   property operator()(const T t) and operator()()
  ////   without changing the interface that clients use

  template <typename T>
  struct Constraint {
    Constraint(T& t) : innerVar(t) {}
    ~Constraint() {}
    virtual void in(const T& t) {
      /*-- add input constraints here --*/
      //std::cout << "\n  checking input constraints";
      innerVar = t;
    }
    virtual T& out() {
      /*-- add output constraints here --*/
      //std::cout << "\n  checking output constraints";
      return innerVar;
    }
    T& innerVar;  // reference to E<T>'s innerVar, passed via constraint
  };              // constructor in Property constructor initialization

  ///////////////////////////////////////////////////////////////////
  // Property class creates getter and setter methods
  // for an encapsulated variable with a single statement

  template <
    typename T, 
    template <class T> class E = Ext, 
    template <class T> class C = Constraint
  >
  class Property : public E<T>
  {
  public:
    Property() : E<T>(), c_(E<T>::innerVar) {}
    Property(T& t) : E<T>(t), c_(t) {}
    virtual ~Property() {}

    //----< setter assigns the state of t to the internal store >----

    virtual void operator()(const T t)
    {
      c_.in(t);  // in may contain input constraint logic
    }

    //----< getter returns the value of the internal store >---------
    /*
    *  - Return by value is used so that clients cannot change the
    *    internal store without using setter.
    */
    virtual T operator()()
    {
      return c_.out();  // out may contain output constraint logic
    }
    //----< getter that returns reference >--------------------------
    /*
    *  - Allows us to call any member function on stored type - that's good.
    *  - Note that this also allows user to bypass constraint logic - that's bad.
    *    So we shouldn't use it normally. ref() is backdoor access to encapsulated
    *    variable and may be removed later.
    *  - The Ext class is provided to allow us to avoid use of ref(), but causes
    *    user more effort that we'd like. See specialization for std::string, 
    *    and std::vector<T>, above.
    */
    virtual T& ref()
    {
      return E<T>::innerVar;
    }
  private:
    C<T> c_;
  };

  template <typename T>
  std::string showType(const Property<T>& t)
  {
    return std::string("\n  encapsulated type is ") + t.typeIs();
  }

  template <typename T>
  std::string showSize(const Property<T>& t)
  {
    return std::string("\n  size of encapsulated type is ") + std::to_string(t.sizeIs());
  }
}
