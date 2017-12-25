#include <iostream>
#include "Utils.hh"

using namespace std;

namespace Utils{
  
  template<typename T>
  void setUnion(const std::set<T> &setA, const std::set<T> &setB, std::set<T> &result){
    result = setA;
    for (auto it = setB.cbegin(); it != setB.cend(); ++it){
      if (result.find(*it) == result.end())
	result.insert(*it);
    }
  }

  template<typename T>
  bool isSubset(const std::set<T> &small, const std::set<T> &big){
    for (auto it = small.cbegin(); it != small.cend(); ++it){
      if (big.find(*it) == big.cend())
	return false;
    }
    return true;
  }

  template<typename T>
  void printSet(const std::set<T> &mySet){
    for (auto it = mySet.cbegin(); it != mySet.cend(); ++it){
      cout << *it << ", ";
    }
    cout << endl;
  }

  template void printSet<int>(const std::set<int> &mySet);
  template void setUnion<int>(const std::set<int> &setA, const std::set<int> &setB, std::set<int> &result);
  template bool isSubset<int>(const std::set<int> &small, const std::set<int> &big);
}
