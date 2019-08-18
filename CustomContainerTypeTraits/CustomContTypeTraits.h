#pragma once
/////////////////////////////////////////////////////////////////////
// CustomContTypeTraits.h - Defines type traits for stl containers //
// ver 1.0 - 18 August 2019                                        //
//-----------------------------------------------------------------//
// Jim Fawcett, Emeritus Teaching Professor, Syracuse University   //
/////////////////////////////////////////////////////////////////////
/*
*  This is a minor extension of work by Trevor Hickey:
*  https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time
*/
/*
* Package Operations:
* -------------------
* This package defines structs used for template specialization:
* - is_stl_container
* - is_stl_seq_container
* - is_stl_assoc_container
*
* Required Files:
* ---------------
* CustomContTypeTraits.h, 
* CustomContTypeTraits.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 18 Aug 2019
* - first release
*/

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <type_traits>

//specialize a type for all of the STL containers.
namespace is_stl_container_impl {

  template <typename T>       struct is_stl_container :std::false_type {};
  template <typename T, std::size_t N> struct is_stl_container<std::array    <T, N>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::vector            <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::deque             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::list              <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::forward_list      <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::set               <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::multiset          <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::map               <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::multimap          <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::stack             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::queue             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>> :std::true_type {};
}

//type trait to use the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
  static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

//specialize a type for all of the STL sequential containers.
namespace is_stl_seq_container_impl {

  template <typename T>       struct is_stl_seq_container :std::false_type {};
  template <typename T, std::size_t N> struct is_stl_seq_container<std::array    <T, N>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::vector            <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::deque             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::list              <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::forward_list      <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::stack             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::queue             <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_seq_container<std::priority_queue    <Args...>> :std::true_type {};
}

//type trait to use the implementation type traits as well as decay the type
template <typename T> struct is_stl_seq_container {
  static constexpr bool const value = is_stl_seq_container_impl::is_stl_seq_container<std::decay_t<T>>::value;
};

//specialize a type for all of the STL associative containers.
namespace is_stl_assoc_container_impl {

  template <typename T>       struct is_stl_assoc_container :std::false_type {};
  template <typename... Args> struct is_stl_assoc_container<std::set               <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::multiset          <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::map               <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::multimap          <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::unordered_set     <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::unordered_multiset<Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::unordered_map     <Args...>> :std::true_type {};
  template <typename... Args> struct is_stl_assoc_container<std::unordered_multimap<Args...>> :std::true_type {};
}

//type trait to use the implementation type traits as well as decay the type
template <typename T> struct is_stl_assoc_container {
  static constexpr bool const value = is_stl_assoc_container_impl::is_stl_assoc_container<std::decay_t<T>>::value;
};
