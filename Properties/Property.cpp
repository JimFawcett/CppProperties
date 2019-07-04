/////////////////////////////////////////////////////////////////////
// Property.cpp - provides getter and setter methods               //
// ver 1.3                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_PROPERTIES

#include "Property.h"
#include <iostream>
#include <string>

using namespace Utilities;

class Widget
{
public:
  Widget(const std::string& msg = "widget here") : msg_(msg) {}
  void say() {
    std::cout << "\n  " << msg_;
  }
private:
  std::string msg_;
};

class X
{
public:
  Property<Box<int>> intProp;      // won't compile without boxing a primitive like int
  Property<Box<double>> dblProp;   // won't compile without boxing
  Property<Widget> wgtProp;        // doesn't need boxing - it's not a primitive
};

int main()
{
  std::cout << "\n  Demonstrating Properties";
  std::cout << "\n ==========================";

  X x;
  x.intProp.verbose(true);
  x.dblProp.verbose(true);
  x.wgtProp.verbose(true);
  Widget widget;
  x.intProp(3);
  std::cout << showType(x.intProp);
  std::cout << showSize(x.intProp);
  x.dblProp(3.14159);
  std::cout << showType(x.dblProp);
  std::cout << showSize(x.dblProp);
  x.wgtProp(widget);
  std::cout << showType(x.wgtProp);
  std::cout << showSize(x.wgtProp);
  std::cout << "\n";

  int saveInt = x.intProp();
  std::cout << "\n  x.intProp = " << saveInt;
  double saveDbl = x.dblProp();
  std::cout << "\n  x.dblProp = " << saveDbl;
  Widget saveWidget = x.wgtProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  saveWidget.say();
  std::cout << "\n";

  std::cout << "\n  after modifying X's properties:";
  std::cout << "\n ---------------------------------";

  x.intProp(-2);
  x.dblProp(0.33333);
  Widget newWgt("newWgt here");
  x.wgtProp(newWgt);
  saveInt = x.intProp();
  std::cout << "\n  x.intProp = " << saveInt;
  saveDbl = x.dblProp();
  std::cout << "\n  x.dblProp = " << saveDbl;
  saveWidget = x.wgtProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  saveWidget.say();
  std::cout << "\n";

  std::cout << "\n  demonstrating string properties";
  std::cout << "\n ---------------------------------";

  Property<std::string> strProp;
  strProp.verbose(true);
  strProp("a string");
  std::string saveStr = strProp();
  std::cout << "\n  value of strProp(\"a string\") is: " << saveStr;
  strProp;
  strProp.operator+=(" with some more text");
  saveStr = strProp();
  std::cout << "\n  after appending to strProp, strProp() yields: " << saveStr;
  std::cout << "\n  the 4th character of strProp is: " << strProp[3];
  std::cout << "\n";

  std::cout << "\n  demonstrating vector properties";
  std::cout << "\n ---------------------------------";

  Property<std::vector<int>> vecProp;
  vecProp.verbose(true);
  vecProp.push_back(3);
  vecProp.push_back(4);
  vecProp.push_back(5);
  std::cout << "\n  contents of vecProp: ";
  for (int item : vecProp)
  {
    std::cout << item << " ";
  }

  vecProp.insert(vecProp.begin(), -5);
  vecProp.insert(vecProp.begin() + 1, -4);
  std::cout << "\n  contents of vecProp: ";
  for (int item : vecProp)
  {
    std::cout << item << " ";
  }

  std::vector<int> temp{ 5, 4, 3, 2, 1, 0, -1, -2 };
  vecProp(temp);
  std::cout << "\n  contents of vecProp: ";
  for (int item : vecProp)
  {
    std::cout << item << " ";
  }

  std::vector<double> temp2{ 0, 0.5, 1.0, 1.5, 2.0 };

  /////////////////////////////////////////////////////////////////////
  // Property<std::vector<double>, Constraint> vecDblProp(temp2, true);
  // - This declaration works too, using explicit template arguments

  Property<std::vector<double>> vecDblProp(temp2, true);
  std::cout << "\n  contents of vecDblProp: ";
  for (auto item : vecDblProp)
  {
    std::cout << item << " ";
  }

  // copy construction works
  Property<std::vector<double>> newVecDblProp = vecDblProp;
  std::cout << "\n  contents of copy of vecDblProp: ";
  for (auto item : newVecDblProp)
  {
    std::cout << item << " ";
  }

  // assignment works because we managed it's C::innerValue
  std::cout << "\n  changing 2nd item to 42.0";
  newVecDblProp[1] = 42.0;
  vecDblProp = newVecDblProp;
  std::cout << "\n  contents of target of assignment: ";
  for (auto item : newVecDblProp)
  {
    std::cout << item << " ";
  }

  std::cout << "\n\n";
}
#endif
