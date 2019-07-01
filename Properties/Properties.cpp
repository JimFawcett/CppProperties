/////////////////////////////////////////////////////////////////////
// Properties.cpp - provides getter and setter methods             //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_PROPERTIES

#include "Properties.h"
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
  Property<int> intProp;
  Property<double> dblProp;
  Property<Widget> wgtProp;
};

int main()
{
  std::cout << "\n  Demonstrating Properties";
  std::cout << "\n ==========================";

  X x;
  Widget widget;
  x.intProp(3);
  std::cout << "\n  " << x.intProp.typeIs();
  x.dblProp(3.14159);
  std::cout << "\n  " << x.dblProp.typeIs();
  x.wgtProp(widget);
  std::cout << "\n  " << x.wgtProp.typeIs();

  std::cout << "\n  x.intProp = " << x.intProp();
  std::cout << "\n  x.dblProp = " << x.dblProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  x.wgtProp().say();
  std::cout << "\n";
  
  std::cout << "\n  after modifying X's properties:";
  std::cout << "\n ---------------------------------";

  x.intProp(-2);
  x.dblProp(0.33333);
  Widget newWgt("newWgt here");
  x.wgtProp(newWgt);
  std::cout << "\n  x.intProp = " << x.intProp();
  std::cout << "\n  x.dblProp = " << x.dblProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  x.wgtProp().say();

  Property<std::string, StrExt<std::string>> strProp;
  strProp("a string");
  std::cout << "\n  value of strProp(\"a string\") is: " << strProp();
  strProp;
  strProp.operator+=(" with some more text");
  std::cout << "\n  after appending to strProp, strProp() yields: " << strProp();
  std::cout << "\n\n";
}
#endif
