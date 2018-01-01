#pragma once

#include <set>
#include <utility>

namespace Utils{
  template<typename T>
  void setUnion(const std::set<T> &setA, const std::set<T> &setB, std::set<T> &result);

  template<typename T>
  void setIntersect(const std::set<T> &setA, const std::set<T> &setB, std::set<T> &result);

  template<typename T>
  bool isSubset(const std::set<T> &small, const std::set<T> &big);

  template<typename T>
  void printSet(const std::set<T> &mySet);

  template<typename T>
  void setDiff(const std::set<T> &in, const std::set<T> &blacklist, std::set<T> &out);
  
  template<typename T>
  void setDiff(std::set<T> &inAndOut, const std::set<T> &blacklist);

  void getNeighbors(std::pair<int,int> point, int range, std::set<std::pair<int,int>> &neighbors, int size);
}
