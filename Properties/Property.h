#pragma once
/////////////////////////////////////////////////////////////////////
// Property.h - provides getter and setter methods                 //
// ver 1.3                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single Property<T, C> class that provides
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
  // Constraint provides inner methods for the property's
  // getter and setter methods.
  // - That allows you to add custom logic for the built in
  //   property operator()(const T t) and operator()()
  //   without changing the interface that clients use
  // - verbose_ is a boolean that controls annotation of method 
  //   flow for debugging

  template <typename T>
  struct Constraint {
    Constraint(T& t, bool verbose = false) : innerVar(t), verbose_(verbose) {}
    ~Constraint() {}
    virtual void in(const T& t) {
      /*-- add input constraints here --*/
      if (verbose_)
        std::cout << "\n  checking input constraints";
      innerVar = t;
    }
    virtual T& out() {
      /*-- add output constraints here --*/
      if (verbose_)
        std::cout << "\n  checking output constraints";
      return innerVar;
    }
    void verbose(bool verbose = true)
    {
      verbose_ = verbose;
    }
    bool verbose_ = false;
    T& innerVar;  // reference to E<T>'s innerVar, passed via constraint
  };              // constructor in Property constructor initialization

  ///////////////////////////////////////////////////////////////////
  // Property class creates getter and setter methods
  // for an encapsulated variable with a single statement

  template <
    typename T,
    template <class T> class C = Constraint
  >
  class Property : public T
  {
  public:
    Property() : T(), c_(*this) {}
    Property(T& t, bool verbose = false) : T(t), c_(t, verbose)
    {
      c_.in(t);  // ensure that t satisfies constraints
    }
    virtual ~Property() {}

    Property<T, C>& operator=(const Property<T, C>& prop)
    {
      if (this == &prop)
        return *this;
      
      // can't assign c_ = prop.c_ because both hold references
      // but can assign the reference values

      c_.innerVar = prop.c_.innerVar;
      return *this;
    }
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

    //----< returns typeid name of encapsulated type >---------------

    std::string typeIs() const
    {
      return typeid(C<T>::innerVar).name();
    }

    //----< returns size of encapsulated type >----------------------

    size_t sizeIs() const
    {
      return sizeof(C<T>::innerVar);
    }

    //----< turns on or off logging constraint ops to console >------

    void verbose(bool verbose)
    {
      c_.verbose(verbose);
    }
  private:
    C<T> c_;  // constraint instance supports adding logic to get, set methods
  };

  //----< display type info on console >----------------------------
  
  template <typename T>
  std::string showType(const Property<T>& t)
  {
    return std::string("\n  encapsulated type is ") + t.typeIs();
  }

  //----< display size info on console >-----------------------------

  template <typename T>
  std::string showSize(const Property<T>& t)
  {
    return std::string("\n  size of encapsulated type is ") + std::to_string(t.sizeIs());
  }

  ///////////////////////////////////////////////////////////////////
  // Box class wraps primitives so they can be inherited by
  // Property<T, C>

  template<typename T>
  class Box
  {
  public:
    Box() : primitive_(T()) {}
    Box(const T& t) : primitive_(t) {}
    operator T& () { return primitive_; }
    T& operator=(const T& t) { primitive_ = t; return primitive_; }
  private:
    T primitive_;
  };
}
