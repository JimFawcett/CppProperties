// Property.cpp

#include "Property.h"
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <unordered_map>
#include <type_traits>

template <typename T>
void show(
  const std::string& msg, 
  T t,
  typename std::enable_if<std::is_fundamental<T>::value, T>::type = 1
)
{
  std::cout << "\n  " << msg << t;
}

template <typename T>
void show(
  const std::string& msg,
  T& t,
  typename std::enable_if<!std::is_fundamental<T>::value, T>::type* = 0
)
{
  std::cout << "\n  " << msg << "\n    ";
  for (auto item : t)
  {
    std::cout << item << " ";
  }
}

int main()
{
  std::cout << "\n  Testing Properties";
  std::cout << "\n ====================\n";

  std::cout << "\n  Testing PropertyBase<int> class";
  std::cout << "\n -----------------------------";
  PropertyBase<int> iProp1 = 6;
  std::cout << "\n  iProp1 = " << iProp1();
  show("iProp1() = ", iProp1());

  PropertyBase<int> iProp2;
  iProp2 = 3;
  std::cout << "\n  iProp2 = " << iProp2();

  PropertyBase<int> iProp3 = iProp1() + iProp2();
  std::cout << "\n  iProp3 = " << iProp3();

  std::cout << "\n\n  Testing TS_Property<int> class";
  std::cout << "\n --------------------------------";
  TS_Property<int> TS_iProp1;
  TS_iProp1(6);
  std::cout << "\n  TS_iProp1 = " << TS_iProp1();
  show("TS_iProp1() = ", TS_iProp1());
  TS_Property<int> TS_iProp1a(6);
  show("TS_iProp1a() = ", TS_iProp1a());

  TS_Property<int> TS_iProp2;
  int test = 3;
  TS_iProp2 = 3;
  std::cout << "\n  TS_iProp2 = " << TS_iProp2();

  int sum = TS_iProp1() + TS_iProp2();
  TS_Property<int> TS_iProp3(sum);
  TS_iProp3 = TS_iProp1() + TS_iProp2();
  std::cout << "\n  TS_iProp3 = " << TS_iProp3();

  std::cout << "\n\n  Testing TS_Property<std::vector<int>>";
  std::cout << "\n ---------------------------------------";
  TS_Property<std::vector<int>> TS_PropVi;
  TS_PropVi.push_back(3);
  TS_PropVi.push_back(2);
  TS_PropVi.push_back(1);
  std::vector<int> vi = TS_PropVi();
  std::vector<int>::iterator iter = vi.begin();
  show("TS_PropVi", TS_PropVi);

  std::cout << "\n\n  erasing 2nd item:";
  iter = TS_PropVi.begin();
  iter = TS_PropVi.erase(iter + 1);
  show("TS_PropVi", TS_PropVi);

  std::cout << "\n\n  inserting 13 at 2nd item:";
  iter = TS_PropVi.begin();
  iter = TS_PropVi.insert(iter + 1, 13);
  show("TS_PropVi", TS_PropVi);
  std::cout << std::endl;

  TS_PropVi.push_back(4);
  show("TS_PropVi:", TS_PropVi);

  TS_PropVi.push_back(2);
  show("TS_PropVi:", TS_PropVi);

  TS_PropVi.push_back(3);
  show("TS_PropVi:", TS_PropVi);

  std::vector<int> testVec;
  testVec.push_back(-7);
  TS_Property<std::vector<int>> TS_PropVi1(testVec);
  show("TS_PropVi1", TS_PropVi1);
  TS_Property<std::vector<int>> TS_PropVi2 = testVec;
  testVec.push_back(18);
  TS_PropVi2 = testVec;
  show("TS_PropVi2", TS_PropVi2);

  TS_PropVi2.lock();
  int val0 = TS_PropVi2[0];
  TS_PropVi2.unlock();
  std::cout << "\n  TS_PropVi2[0] = " << val0;
  std::cout << "\n  after TS_PropVi2[1] = -19:";
  TS_PropVi2.lock();
  TS_PropVi2[1] = -19;
  TS_PropVi2.unlock();
  show("TS_PropVi2", TS_PropVi2);

  std::cout << "\n\n  Testing TS_Property<std::deque<double>>";
  std::cout << "\n -----------------------------------------";
  TS_Property<std::deque<double>> TS_PropDd;
  TS_PropDd.push_front(3.14159);
  TS_PropDd.push_back(0.3333);
  TS_PropDd.push_front(-3.333);
  show("TS_PropDd:", TS_PropDd);
  std::cout << "\n  TS_PropDd.size() = " << TS_PropDd.size();
  
  std::cout << "\n\n  TS_PropDd.pop_front";
  std::cout << "\n --------------------";
  TS_PropDd.pop_front();
  show("TS_PropDd:", TS_PropDd);

  std::cout << "\n\n  TS_PropDd.pop_back";
  std::cout << "\n -------------------";
  TS_PropDd.pop_back();
  show("TS_PropDd:", TS_PropDd);
  double d = TS_PropDd.front();
  show("TS_PropDd.front() = ", d);

  //std::cout << "\n\n  Testing TS_Property<std::unordered_map<std::string, std::string>>";
  //std::cout << "\n -------------------------------------------------------------------";
  //TS_Property<std::unordered_map<std::string, std::string>> TS_UnordMap1;
  //std::pair<std::string, std::string> item1 { "one", "1" };
  //std::pair<std::string, std::string> item2 { "two", "2" };
  //std::unordered_map<std::string, std::string>::iterator iter2 = TS_UnordMap1.begin();
  //TS_UnordMap1.insert(iter2, item1);
  //TS_UnordMap1.insert(iter2, item2);

  //show("TS_UnordMap1", TS_UnordMap1);
  int debug = 0;
  //std::cout << TS_UnordMap1[0];
  //TS_PropDd.push_front(3.14159);
  //TS_PropDd.push_back(0.3333);
  //TS_PropDd.push_front(-3.333);
  //show("TS_PropDd:", TS_PropDd);
  //std::cout << "\n  TS_PropDd.size() = " << TS_PropDd.size();
  
  std::cout << "\n\n";
}