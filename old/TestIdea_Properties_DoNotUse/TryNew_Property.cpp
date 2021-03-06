/////////////////////////////////////////////////////////////////////
// TryNew_Property.cpp - provides getter and setter methods        //
// ver 1.0                                                         //
// Jim Fawcett, github repositories, Summer 2019                   //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_TRYNEW_PROPERTY

#include "TryNew_Property.h"
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

//  Property<std::string, Ext<std::string>> strProp;
  Property<std::string> strProp;
  strProp.verbose(true);
  strProp("a string");
  std::cout << "\n  value of strProp(\"a string\") is: " << strProp();
  strProp;
  strProp.operator+=(" with some more text");
  std::cout << "\n  after appending to strProp, strProp() yields: " << strProp();
  std::cout << "\n  the 4th character of strProp is: " << strProp[3];
  std::cout << "\n";

  //Property < std::vector<int>, Ext<std::vector<int>>> vecProp;
  Property<std::vector<int>> vecProp;
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
  // Property<std::vector<double>, Ext, Constraint> vecDblProp(temp2);
  // - This declaration works too, using explicit template arguments

  Property<std::vector<double>> vecDblProp(temp2);
  std::cout << "\n  contents of vecDblProp: ";
  for (auto item : vecDblProp)
  {
    std::cout << item << " ";
  }

  std::cout << "\n\n";
}
#endif
