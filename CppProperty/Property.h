#pragma once
/////////////////////////////////////////////////////////////////////
// Property.h - Implements properties for C++                      //
// ver 2.0 - 17 August 2019                                        //
//-----------------------------------------------------------------//
// Jim Fawcett (c) copyright 2019                                  //
// All rights granted provided this copyright notice is retained   //
//-----------------------------------------------------------------//
// Jim Fawcett, Emeritus Teaching Professor, Syracuse University   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This Property package provides classes:
* - PropContainer<T>
*   Provides virtual methods get(), set(t), lock(), unlock() 
* - PropertyBase<T>
*   Provides user methods:
*     PropertyBase<T>& operator=(const T& t)
*     void operator()(const T& t)
*     T operator()()
* - PushAndPopProperty<T, Enabler=void>
*     Intended for STL containers, e.g., PushAndPopProperty<std::vector<int>>
*     Provides a lot of the STL methods like push_back(const T& t)
* - PushAndPopProperty<T, std::enable_if_t<std::is_arithmetic<T>::value>>
*     A specialization for fundamental data, e.g., int, double, ...
* - TS_Property<T>
*     A thread-safe version of PushAndPopProperty<T>
*
* Required Files:
* ---------------
* Property.h, Property.cpp
*
* Maintenance History:
* --------------------
* ver 2.0 : 18 Aug 2019
* - completely new design - better structure, safer functionaligy
* ver 1.0 : 03 Jun 2019
* - first release
*
*/

#include <thread>
#include <mutex>
#include <type_traits>

///////////////////////////////////////////////////////////////
// PropContainer<T> class
// - manages instance of the property

template <typename T>
class PropContainer {
public:
  //----< override to provide locking >--------------------

  virtual void lock()
  {
    std::cout << "\n---- locking ----";
    // lock
  }
  //----< override to provide locking >--------------------

  virtual void unlock()
  {
    std::cout << "\n---- unlocking ----";
    // unlock 
  }

  PropContainer() :t_(T()) {}
  PropContainer(const T& t)
  {
    set(t);
  }
  virtual ~PropContainer() {}

protected:

  //----< override to provide value management >-----------

  virtual void set(const T& t)
  {
    std::cout << "\n---- setting ----";
    t_ = t;
  }
  //----< override to provide value management >-----------

  virtual T& get()
  {
    std::cout << "\n---- getting ----";
    return t_;
  }

protected:
  T t_;
};

///////////////////////////////////////////////////////////////
// PropertyBase<T> class
// - provides user interface

template <typename T>
class PropertyBase : public PropContainer<T>
{
public:
  PropertyBase() {}
  PropertyBase(const T& t) 
  {
    this->set(t);
  }
  virtual ~PropertyBase() {}

  PropertyBase<T>(const PropertyBase<T>& prop) = delete;
  PropertyBase<T>& operator=(const PropertyBase<T>& prop) = delete;

  PropertyBase<T>& operator=(const T& t)
  {
    this->set(t);
    return *this;
  }
  void operator()(const T& t)
  {
    this->set(t);
  }
  T operator()()
  {
    return this->get();
  }
protected:
};

///////////////////////////////////////////////////////////////
// PushAndPopProperty<T> class
// - adds methods to interact with STL intances
// - Enabler template paramter is just to manage specialization
//

template<class T, class Enabler = void>
class PushAndPopProperty : public PropertyBase<T>
{
public:
  using iterator = typename T::iterator;

  PushAndPopProperty() {}
  PushAndPopProperty(const T& t) /*: PropertyBase<T>(t) {}*/
  {
    this->set(t);
  }
  PushAndPopProperty<T>& operator=(const T& t)
  {
    //std::cout << "\n-------- calling operator=(const T& t) ----------";
    this->set(t);
    return *this;
  }

  iterator begin() {
    T& t = (*this).get();
    this->lock();
    iterator beg = t.begin();
    this->unlock();
    return beg;
  }

  iterator end() {
    T& t = (*this).get();
    this->lock();
    iterator end = t.end();
    this->unlock();
    return end;
  }

  size_t size()
  {
    T& t = this->get();
    this->lock();
    size_t sz = t.size();
    this->unlock();
    return sz;
  }

  typename iterator insert(iterator iter, typename const T::value_type & value)
  {
    T& t = this->get();
    this->lock();
    iterator curr = t.insert(iter, value);
    this->unlock();
    return curr;
  }

  typename iterator erase(iterator iter)
  {
    T& t = this->get();
    this->lock();
    iterator next = t.erase(iter);
    this->unlock();
    return next;
  }

  typename T::value_type operator[](int n) const
  {
    PushAndPopProperty<T>* pPAPP = const_cast<PushAndPopProperty<T>*>(this);
    T& t = pPAPP->get();
    //pPAPP->lock();
    typename T::value_type v = t[n];
    //pPAPP->unlock();
    return v;
  }
  /*
  *  This non-const operator[] is not thread safe because it returns a
  *  reference to an item in the collection.  It's use should be
  *  embedded with calls to lock() and unlock(), and the returned
  *  reference should not be used outside that embedding.
  */
  typename T::value_type& operator[](int n)
  {
    PushAndPopProperty<T>* pPAPP = const_cast<PushAndPopProperty<T>*>(this);
    T& t = pPAPP->get();
    typename T::value_type& v = t[n];
    return v;
  }

  typename T::value_type top()
  {
    T& t = (*this).get();
    this->lock();
    typename T::value_type top = t.top();
    this->unlock();
    return top;
  }

  void push(typename const T::value_type& v)
  {
    T& t = (*this).get();
    this->lock();
    t.push(v);
    this->unlock();
  }

  void pop()
  {
    T& t = (*this).get();
    this->lock();
    t.pop();
    this->unlock();
  }

  void push_back(typename const T::value_type& v)
  {
    T& t = (*this).get();
    this->lock();
    t.push_back(v);
    this->unlock();
  }

  void push_front(typename const T::value_type& v)
  {
    T& t = (*this).get();
    this->lock();
    t.push_front(v);
    this->unlock();
  }

  typename T::value_type front()
  {
    T& t = (*this).get();
    this->lock();
    typename T::value_type front = t.front();
    this->unlock();
    return front;
  }

  typename T::value_type back()
  {
    T& t = (*this).get();
    this->lock();
    typename T::value_type back = t.back();
    this->unlock();
    return back;
  }

  void pop_back()
  {
    T& t = (*this).get();
    this->lock();
    t.pop_back();
    this->unlock();
  }

  void pop_front()
  {
    T& t = (*this).get();
    this->lock();
    t.pop_front();
    this->unlock();
  }
};

///////////////////////////////////////////////////////////////
// PushAndPopProperty<T, std::enable_if_<...>>> class
// - Specializes away all of the pushes and pops etc.
// - Second template parameter selects for fundamental types
//

template<class T>
class PushAndPopProperty<T, std::enable_if_t<std::is_fundamental<T>::value>> : public PropertyBase<T>
{
public:
  PushAndPopProperty() {}
  PushAndPopProperty(const T& t) /*: Property<T>(t) {}*/
  {
    this->set(t);
  }

  PushAndPopProperty<T>& operator=(const T& t)
  {
    this->set(t);
    return *this;
  }
};

///////////////////////////////////////////////////////////////
// TS_Property<T> class
// - Provides type-safe operations for all but indexing
//   and iteration.
// - For those you need to embedd them between lock() and
//   unlock() calls.
//

template<typename T>
class TS_Property : public PushAndPopProperty<T>
{
public:
  TS_Property() : pMtx(new std::recursive_mutex), lck(*pMtx) 
  {
    testLock();
  }
  TS_Property(const T& t) : pMtx(new std::recursive_mutex), lck(*pMtx)
  {
    testLock();
    this->set(t);
  }
  ~TS_Property() 
  {
    pMtx->unlock();
    delete pMtx; 
  }

  void operator=(const T& t)
  {
    this->set(t);
  }
  virtual void lock() override
  {
    //std::cout << "\n  ---- taking lock ----";
    lck.lock();
  }
  virtual void unlock() override
  {
    //std::cout << "\n  ---- releasing lock ----";
    lck.unlock();
  }

protected:

  //----< override to provide value management >-----------

  virtual void set(const T& t) override
  {
    //std::cout << "\n---- setting ----";
    this->t_ = t;
  }
  //----< override to provide value management >-----------

  virtual T& get() override
  {
    //std::cout << "\n---- getting ----";
    // methods that call get() need to setGuards

    return this->t_;
  }

  bool testLock()
  {
    if (pMtx->try_lock() == true)
    {
      //std::cout << "\n  ---- not previously locked ----";
      lck.unlock();
      return false;
    }
    else
    {
      //std::cout << "\n  ---- previously locked ----";
      return true;
    }
  }

  std::recursive_mutex* pMtx;
  std::unique_lock<std::recursive_mutex> lck;
};
