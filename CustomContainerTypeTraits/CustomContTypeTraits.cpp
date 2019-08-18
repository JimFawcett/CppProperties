///////////////////////////////////////////////////////////////////////
// CustomContTypeTraits.cpp - Defines type traits for stl containers //
// ver 1.0 - 18 August 2019                                          //
//-------------------------------------------------------------------//
// Jim Fawcett, Emeritus Teaching Professor, Syracuse University     //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "CustomContTypeTraits.h"

int main()
{
  std::cout << "\n  Testing CustomTraits";
  std::cout << "\n ======================";

  std::cout << std::boolalpha;

  std::cout << "\n  is_stl_container<std::vector<int>>::value: ";
  std::cout << is_stl_container<std::vector<int>>::value;

  std::cout << "\n  is_stl_container<std::unordered_map<std::string,std::string>>::value: ";
  std::cout << is_stl_container<std::unordered_map<std::string, std::string>>::value;

  std::cout << "\n  is_stl_container<int>::value: ";
  std::cout << is_stl_container<int>::value;
  std::cout << std::endl;

  std::cout << "\n  is_stl_seq_container<std::vector<int>>::value: ";
  std::cout << is_stl_seq_container<std::vector<int>>::value;

  std::cout << "\n  is_stl_seq_container<std::unordered_map<std::string,std::string>>::value: ";
  std::cout << is_stl_seq_container<std::unordered_map<std::string, std::string>>::value;

  std::cout << "\n  is_stl_seq_container<int>::value: ";
  std::cout << is_stl_seq_container<int>::value;
  std::cout << std::endl;

  std::cout << "\n  is_stl_assoc_container<std::vector<int>>::value: ";
  std::cout << is_stl_assoc_container<std::vector<int>>::value;

  std::cout << "\n  is_stl_assoc_container<std::unordered_map<std::string,std::string>>::value: ";
  std::cout << is_stl_assoc_container<std::unordered_map<std::string, std::string>>::value;

  std::cout << "\n  is_stl_assoc_container<int>::value: ";
  std::cout << is_stl_assoc_container<int>::value;

  std::cout << "\n\n";
  return 0;
}

