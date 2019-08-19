// Property.cpp

#include "Property.h"
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <unordered_map>
#include <type_traits>


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

  std::cout << "\n\n  Testing Property<int> class";
  std::cout << "\n -----------------------------";
  Property<int> P_iProp1;
  P_iProp1(6);
  std::cout << "\n  P_iProp1 = " << P_iProp1();
  show("P_iProp1() = ", P_iProp1());
  Property<int> P_iProp1a(6);
  show("P_iProp1a() = ", P_iProp1a());

  Property<int> P_iProp2;
  int test = 3;
  P_iProp2 = 3;
  std::cout << "\n  P_iProp2 = " << P_iProp2();

  int sum = P_iProp1() + P_iProp2();
  Property<int> P_iProp3(sum);
  P_iProp3 = P_iProp1() + P_iProp2();
  std::cout << "\n  P_iProp3 = " << P_iProp3();

  std::cout << "\n\n  Testing TS_Property<int> class";
  std::cout << "\n --------------------------------";
  TS_Property<int> TS_iProp1;
  TS_iProp1(6);
  std::cout << "\n  TS_iProp1 = " << TS_iProp1();
  show("TS_iProp1() = ", TS_iProp1());
  TS_Property<int> TS_iProp1a(6);
  show("TS_iProp1a() = ", TS_iProp1a());

  TS_Property<int> TS_iProp2;
  test = 3;
  TS_iProp2 = 3;
  std::cout << "\n  TS_iProp2 = " << TS_iProp2();

  sum = TS_iProp1() + TS_iProp2();
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
  show("TS_PropVi", TS_PropVi());

  std::cout << "\n\n  TS_PropVi.erase(TS_PropVi.begin() + 1):";
  iter = TS_PropVi.begin();
  iter = TS_PropVi.erase(iter + 1);
  show("TS_PropVi", TS_PropVi());

  std::cout << "\n\n  TS_PropVi.insert(TS_PropVi.begin() + 1, 13):";
  iter = TS_PropVi.begin();
  iter = TS_PropVi.insert(iter + 1, 13);
  show("TS_PropVi", TS_PropVi());
  std::cout << std::endl;

  std::cout << "\n\n  TS_PropVi.push_back(4):";
  TS_PropVi.push_back(4);
  show("TS_PropVi:", TS_PropVi());

  std::cout << "\n\n  TS_PropVi.push_back(2):";
  TS_PropVi.push_back(2);
  show("TS_PropVi:", TS_PropVi());

  std::cout << "\n\n  TS_PropVi.push_back(3):";
  TS_PropVi.push_back(3);
  show("TS_PropVi:", TS_PropVi());

  std::cout << "\n\n  creating a std::vector<int>, testVec";
  std::vector<int> testVec;
  testVec.push_back(-7);

  std::cout << "\n  TS_PropVi(testVec):";
  TS_Property<std::vector<int>> TS_PropVi1(testVec);
  show("TS_PropVi1", TS_PropVi1());

  std::cout << "\n\n  TS_PropVi2 = testVec";
  TS_Property<std::vector<int>> TS_PropVi2 = testVec;
  std::cout << "\n  push_back(18)";
  testVec.push_back(18);
  TS_PropVi2 = testVec;
  show("TS_PropVi2", TS_PropVi2());

  std::cout << "\n\n  TS_PropVi2.lock()";
  std::cout << "\n  int val0 = TS_PropVi2[0]";
  std::cout << "\n  TS_PropVi2.unlock()";
  TS_PropVi2.lock();
  int val0 = TS_PropVi2[0];
  TS_PropVi2.unlock();
  std::cout << "\n  TS_PropVi2[0] = " << val0;
  std::cout << "\n  after TS_PropVi2[1] = -19:";
  TS_PropVi2.lock();
  TS_PropVi2[1] = -19;
  TS_PropVi2.unlock();
  show("TS_PropVi2", TS_PropVi2());

  std::cout << "\n\n  Testing TS_Property<std::deque<double>>";
  std::cout << "\n -----------------------------------------";
  TS_Property<std::deque<double>> TS_PropDd;
  TS_PropDd.push_front(3.14159);
  TS_PropDd.push_back(0.3333);
  TS_PropDd.push_front(-3.333);
  show("TS_PropDd:", TS_PropDd());
  std::cout << "\n  TS_PropDd.size() = " << TS_PropDd.size();
  
  std::cout << "\n\n  TS_PropDd.pop_front";
  std::cout << "\n --------------------";
  TS_PropDd.pop_front();
  show("TS_PropDd:", TS_PropDd());

  std::cout << "\n\n  TS_PropDd.pop_back";
  std::cout << "\n -------------------";
  TS_PropDd.pop_back();
  show("TS_PropDd:", TS_PropDd());
  double d = TS_PropDd.front();
  show("TS_PropDd.front() = ", d);

  std::cout << "\n\n  Testing TS_Property<std::unordered_map<std::string, std::string>>";
  std::cout << "\n -------------------------------------------------------------------";
  TS_Property<std::unordered_map<std::string, std::string>> TS_UnordMap1;
  std::pair<std::string, std::string> item1 { "one", "1" };
  std::pair<std::string, std::string> item2{ "two", "2" };
  std::pair<std::string, std::string> item3{ "three", "3" };
  std::pair<std::string, std::string> item4{ "four", "4" };
  TS_UnordMap1.insert(item1);
  TS_UnordMap1.insert(item2);
  TS_UnordMap1.insert(item3);
  std::unordered_map<std::string, std::string>::iterator iter2 = TS_UnordMap1.begin();
  TS_UnordMap1.insert(iter2, item4);  // iterator is a hint
  show("TS_UnordMap1", TS_UnordMap1());

  std::cout << "\n\n  TS_UnordMap1.erase(TS_UnordMap1.begin())";
  iter2 = TS_UnordMap1.begin();
  TS_UnordMap1.erase(iter2);
  show("TS_UnordMap1", TS_UnordMap1());
  iter2 = ++TS_UnordMap1.begin();

  std::cout << "\n\n  TS_UnordMap1.insert({ \"five\", \"5\" })";
  std::pair<std::string, std::string> item5{ "five", "5" };
  TS_UnordMap1.insert(item5);
  show("TS_UnordMap1", TS_UnordMap1());

  std::unordered_map<std::string, std::string>::key_type key = "five";

  std::cout << "\n\n  TS_UnordMap1.find(\"" << key << "\")";
  std::unordered_map<std::string, std::string>::const_iterator iter3;
  iter3 = TS_UnordMap1.find(key);
  if (iter3 != TS_UnordMap1.end())
  {
    std::cout << "\n  found item with key = \"" << key << "\"";
    std::cout << "\n  value is \"" << iter3->second << "\"";
  }
  else
    std::cout << "\n  did not find item with key = \"" << key << "\"";

  key = "foobar";
  std::cout << "\n\n  TS_UnordMap1.find(\"" << key << "\")";
  iter3 = TS_UnordMap1.find(key);
  if (iter3 != TS_UnordMap1.end())
  {
    std::cout << "\n  found item with key = \"" << key << "\"";
    std::cout << "\n  value is \"" << iter3->second << "\"";
  }
  else
    std::cout << "\n  did not find item with key = \"" << key << "\"";

  std::unordered_map<std::string, std::string>::mapped_type value;

  try {
    key = "five";
    std::cout << "\n\n  TS_UnordMap1[\"" << key << "\"] returned ";
    value = TS_UnordMap1[key];
    std::cout << "\"" << value << "\"";

    key = "foobar";
    std::cout << "\n\n  TS_UnordMap1[\"" << key << "\"] returned ";
    value = TS_UnordMap1[key];
    std::cout << "\"" << value << "\"";

  }
  catch (std::exception& ex)
  {
    std::cout << "\n  " << ex.what();
  }

  std::cout << "\n\n  method editItem(key, valule) is used instead of TS_UnordMap1[key] = value";
  std::cout << "\n    that is not implemented";
  key = "foobar";
  value = "feebar";
  std::cout << "\n\n  TS_UnordMap1.editItem(\"" << key << ", " << value << "\")";
  TS_UnordMap1.editItem(key, "feebar");
  show("TS_UnordMap1", TS_UnordMap1());

  value = "foobar";
  std::cout << "\n\n  TS_UnordMap1.editItem(\"" << key << ", " << value << "\")";
  TS_UnordMap1.editItem(key, "foobar");
  show("TS_UnordMap1", TS_UnordMap1());

  std::cout << "\n\n  ---- That's all folks! ----";
  std::cout << "\n\n";
}