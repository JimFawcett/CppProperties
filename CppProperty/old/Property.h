#pragma once
// Property.h

template <typename T>
class NoConstraints {
public:
  NoConstraints() :t_(T()) {}
  NoConstraints(T t) 
  {
    set(t);
  }
  void set(T t)
  {
    t_ = t;
  }
  T& get()
  {
    return t_;
  }
protected:
  T& ref() { return t_; }
  T t_;
};

template <
  typename T, 
  template<class T> class C = NoConstraints
>
class Property
{
public:
  Property() : c_() {}
  Property(const T& t) : c_(t) {}
  virtual ~Property() {}
  Property<T, C>& operator=(const Property<T, C>& prop)
  {
    if (this == &prop)
      return *this;
    c_.set(prop());
    return *this;
  }
  Property<T, C>& operator=(const T& t)
  {
    c_.set(t);
    return *this;
  }
  void operator()(T& t)
  {
    c_.set(t);
  }
  T operator()()
  {
    return c_.get();
  }
protected:
  C<T> c_;
  T& ref() { return c_.get(); }
};

template<class T, template <class T> class C = NoConstraints>
class PushAndPopProperty : public Property<T, C>
{
public:
  using iterator = typename T::const_iterator;

  iterator begin() {
    T& t = (*this).ref();
    return t.begin();
  }

  iterator end() {
    T& t = (*this).ref();
    return t.end();
  }

  typename T::value_type top()
  {
    T& t = (*this).ref();
    return t.top();
  }

  void push(typename const T::value_type& v)
  {
    T& t = (*this).ref();
    t.push(v);
  }

  void pop()
  {
    T& t = (*this).ref();
    t.pop();
  }

  void push_back(typename const T::value_type& v)
  {
    T& t = (*this).ref();
    t.push_back(v);
  }

  void push_front(typename const T::value_type& v)
  {
    T& t = (*this).ref();
    t.push_front(v);
  }

  typename T::value_type front()
  {
    T& t = (*this).ref();
    return t.front();
  }

  typename T::value_type back()
  {
    T& t = (*this).ref();
    return t.back();
  }

  void pop_back()
  {
    T& t = (*this).ref();
    t.pop_back();
  }

  void pop_front()
  {
    T& t = (*this).ref();
    t.pop_front();
  }
private:
  //T t_;
};

#include <stack>

template<class T, template <class T> class C = NoConstraints>
class stackProperty : public Property<std::stack<T>, C>
{
public:
  typename T top()
  {
    std::stack<T>& stk = (*this).ref();
    return stk.top();
  }

  void push(typename const T& v)
  {
    std::stack<T>& stk = (*this).ref();
    stk.push(v);
  }

  void pop()
  {
    std::stack<T>& stk = (*this).ref();
    stk.pop();
  }
private:
  //T t_;
};
