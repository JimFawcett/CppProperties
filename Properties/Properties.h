#pragma once
/////////////////////////////////////////////////////////////////////
// Properties.h - provides getter and setter methods               //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single Property<T> class that provides
*  getter and setter methods and backing store T t_ using the property 
*  name.
*  - Here's an example:
*      Property<int> intProp;
*      intProp(42)    Sets its internal store to 42
*      intProp()      returns the internal store value
*  - So one declaration creates two methods with simple syntax, using
*    the property's instance name. 
*  - The getter and setter methods are virtual, so you can override
*    them to provide logic that constraints getting and setting.
*  - Property also provides the method ref() which returns a reference
*    to the underlying stored value.  That lets us call member functions
*    directly on the stored type.  However, that lets users bypass
*    setter logic, so use it with caution.
*
*  Required Files:
*  ---------------
*  Properties.h
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 30 Jan 2019
*  - added ref()
*  ver 1.0 : 11 Oct 2018
*  - first release
*/

#include <string>

namespace Utilities
{
  ///////////////////////////////////////////////////////////////////
  // Ext provides a custom extension interface for properties
  // - You can define any methods that are syntactically correct
  //   for the type T
  // - Example: push_back when T is an STL container with method push_back 

  template <typename T>
  struct Ext {
    Ext() : t_(T()) {}
    Ext(const T& t) : t_(t) {}
    std::string typeIs()
    {
      return std::string("encapsulated type is ") + typeid(t_).name();
    }
    T t_;
  };

  template <typename T>
  struct StrExt {
    StrExt() : t_(T()) {
      //std::cout << "\n  constructing new StrExt instance";
    }
    StrExt(const T& t) : t_(t) {
      //std::cout << "\n  constructing new StrExt instance";
    }
    void operator+=(const T& t)  // no chaining  because shouldn't return reference
    {
      t_ += t;
    }
    T t_;
  };

  ///////////////////////////////////////////////////////////////////
  // Constraint provides inner methods for the property's
  // getter and setter methods.
  // - That allows you to add custom logic for the built in
  //   property operator()(const T t) and operator()()
  //   without changing the interface that clients use

  template <typename T>
  struct Constraint {
    Constraint(T& t) : t_(t) {}
    ~Constraint() {}
    virtual void in(const T& t) {
      /*-- add input constraints here --*/
      t_ = t;
    }
    virtual T& out() {
      /*-- add output constraints here --*/
      return t_;
    }
    T& t_;
  };

  ///////////////////////////////////////////////////////////////////
  // Property class creates getter and setter methods
  // for an encapsulated variable with a single statement

  template <typename T, typename E = Ext<T>, typename C = Constraint<T>>
  class Property : public E
  {
  public:
    Property() : E(), c_(E::t_) {}
    Property(const T& t) : E(t), c_(t) {}
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
    *  - Allows us to call any member function on stored type
    *  - Note that this allows a user to bypass all logic in setter.
    *  - The Ext class is provided to allow us to avoid use of ref()
    */
    virtual T& ref()
    {
      return E::t_;
    }
  private:
    C c_;
  };
}
