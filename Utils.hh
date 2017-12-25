#pragma once

#include <set>

namespace Utils{
  template<typename T>
  void setUnion(const std::set<T> &setA, const std::set<T> &setB, std::set<T> &result);

  template<typename T>
  bool isSubset(const std::set<T> &small, const std::set<T> &big);

  template<typename T>
  void printSet(const std::set<T> &mySet);
}
