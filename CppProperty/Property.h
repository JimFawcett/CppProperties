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
* - PropertyOps<T, Enabler=void>
*     Intended for STL containers, e.g., PropertyOps<std::vector<int>>
*     Provides a lot of the STL methods like push_back(const T& t)
* - PropertyOps<T, std::enable_if_t<std::is_arithmetic<T>::value>>
*     A specialization for fundamental data, e.g., int, double, ...
* - TS_Property<T>
*     A thread-safe version of PropertyOps<T>
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
#include <iostream>
#include "../CustomContainerTypeTraits/CustomContTypeTraits.h"

///////////////////////////////////////////////////////////////
// PropContainer<T> class
// - manages instance of the property

template <typename T>
class PropContainer {
public:
  //----< override to provide locking >--------------------

  virtual void lock()
  {
    std::cout << "\n---- called empty lock() ----";
  }
  //----< override to provide locking >--------------------

  virtual void unlock()
  {
    std::cout << "\n---- called empty unlock() ----";
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
    lock();
    t_ = t;
    unlock();
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
    // only one copy here due to return value optimization
    this->lock();
    T temp = this->get();
    this->unlock();
    return temp;
  }
protected:
};

///////////////////////////////////////////////////////////////
// PropertyOps<T> class
// - adds methods to interact with STL intances
// - Enabler template paramter is just to manage specialization
//

template<class T, class Enabler = void>
class PropertyOps : public PropertyBase<T>
{
public:
  PropertyOps() {}
  PropertyOps(const T& t) /*: Property<T>(t) {}*/
  {
    this->set(t);
  }

  PropertyOps<T>& operator=(const T& t)
  {
    this->set(t);
    return *this;
  }
};

///////////////////////////////////////////////////////////////
// PropertyOps<T, std::enable_if_<...>>> class
// - Specializes for STL sequence containers with pushes and pops etc.
// - Second template parameter selects for STL sequence containers
//

template<class T>
class PropertyOps<T, std::enable_if_t<is_stl_seq_container<T>::value>> : public PropertyBase<T>
{
public:
  using iterator = typename T::iterator;

  PropertyOps() {}
  PropertyOps(const T& t)
  {
    this->set(t);
  }
  PropertyOps<T>& operator=(const T& t)
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
    PropertyOps<T>* pPAPP = const_cast<PropertyOps<T>*>(this);
    T& t = pPAPP->get();
    pPAPP->lock();
    typename T::value_type v = t[n];
    pPAPP->unlock();
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
    PropertyOps<T>* pPAPP = const_cast<PropertyOps<T>*>(this);
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
// PropertyOps<T, std::enable_if_<...>>> class
// - Specializes away all of the pushes and pops etc.
// - Second template parameter selects for associative containers
// - A few more operations would be useful like operator[]

template<class T>
class PropertyOps<T, std::enable_if_t<is_stl_assoc_container<T>::value>> : public PropertyBase<T>
{
public:
  using iterator = typename T::iterator;
  using const_iterator = typename T::const_iterator;
  using key_type = typename T::key_type;
  using mapped_type = typename T::mapped_type;
  using value_type = typename T::value_type;

  PropertyOps() {}
  PropertyOps(const T& t)
  {
    this->set(t);
  }

  PropertyOps<T>& operator=(const T& t)
  {
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

  typename iterator insert(iterator iter, typename const T::value_type& value)
  {
    T& t = this->get();
    this->lock();
    iterator curr = t.insert(iter, value);
    this->unlock();
    return curr;
  }

  typename auto insert(typename const T::value_type& value)
  {
    T& t = this->get();
    this->lock();
    auto curr = t.insert(value);
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

  typename const_iterator find(const key_type& key)
  {
    T& t = this->get();
    this->lock();
    const_iterator found = t.find(key);
    this->unlock();
    return found;
  }

  bool contains(const key_type& key) const
  {
    PropertyOps<T>* pPAPP = const_cast<PropertyOps<T>*>(this);
    T& t = pPAPP->get();
    pPAPP->lock();
    const_iterator found = t.find(key);
    iterator end = t.end();
    pPAPP->unlock();
    return found != end;
  }

  const typename T::mapped_type operator[](const key_type& key) const
  {
    key_type savedKey = key;

    PropertyOps<T>* pPAPP = const_cast<PropertyOps<T>*>(this);
    T& t = pPAPP->get();
    if (!contains(key))
    {
      std::invalid_argument exc("exception: key not found");
      throw(exc);
    }
    pPAPP->lock();
    const typename T::mapped_type v = t[key];
    pPAPP->unlock();
    return v;
  }
  /*
  * - This method is a replacement for mapProperty[key] = value.
  * - I wasn't able to figure out how to implement in a
  *   reasonable amount of time
  * - If key does not exist, a new item { key, value } is inserted.
  *   and method returns false (was created)
  * - If key does exist, its mapped value is set to value
  *   and method returns true (was edited)
  */
  bool editItem(const key_type& key, const mapped_type& value)
  {
    T& t = this->get();
    std::pair<key_type, mapped_type> item;
    item.first = key;
    item.second = value;
    iterator iter = t.find(key);
    bool rtn = true;
    if (iter == t.end())
    {
      bool rtn = false;
      insert(item);
    }
    else
    {
      iter->second = value;
    }
    return rtn;
  }
};

///////////////////////////////////////////////////////////////
// Property<T> class
//

template<typename T>
class Property : public PropertyOps<T>
{
public:
  Property() {}
  Property(const T& t)
  {
    this->set(t);
  }
  ~Property() {}

  void operator=(const T& t)
  {
    this->set(t);
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
    return this->t_;
  }
  //----< override to remove lock enunciation >------------

  virtual void lock() override
  {
  
  }
  //----< override to remove unlock enunciation >----------

  virtual void unlock() override
  {

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
class TS_Property : public PropertyOps<T>
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

///////////////////////////////////////////////////////////
// function templates that overload on type traits
//   The technique used here was described by Eli Bendersky:
//   https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
//

template <typename T, typename std::enable_if<std::is_fundamental<T>::value, T>::type* = nullptr>
void show(
  const std::string & msg,
  const T & t
)
{
  std::cout << "\n  " << msg << t;
}

template <typename T, typename std::enable_if<is_stl_seq_container<T>::value, T>::type* = nullptr>
void show(
  const std::string & msg,
  const T & t
)
{
  std::cout << "\n  " << msg << "\n    ";
  for (auto item : t)
  {
    std::cout << item << " ";
  }
}

template <typename T, typename std::enable_if<is_stl_assoc_container<T>::value, T>::type* = nullptr>
void show(
  const std::string & msg,
  const T & t
)
{
  std::cout << "\n  " << msg << "\n    ";
  for (auto item : t)
  {
    std::cout << "{" << item.first << ", " << item.second << "} ";
  }
}
