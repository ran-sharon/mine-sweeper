#include <iostream>
#include "Utils.hh"

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
  void setIntersect(const std::set<T> &setA, const std::set<T> &setB, std::set<T> &result){
    result.clear();
    for (auto it = setA.cbegin(); it != setA.cend(); ++it){
      if (setB.find(*it) != setB.cend())
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
      std::cout << *it << ", ";
    }
    std::cout << std::endl;
  }

  template<typename T>
  void setDiff(const std::set<T> &in, const std::set<T> &blacklist, std::set<T> &out){
    out = in;
    for (auto it = blacklist.cbegin(); it != blacklist.cend(); ++it){
      out.erase(*it);
    }
  }
  
  template<typename T>
  void setDiff(std::set<T> &inAndOut, const std::set<T> &blacklist){    
    for (auto it = blacklist.cbegin(); it != blacklist.cend(); ++it){
      inAndOut.erase(*it);
    }
  }

  void getNeighbors(pair<int,int> point, int range, set<pair<int,int>> &neighbors, int size){
    neighbors.clear();
    int x = point.first;
    int y = point.second;
    for (int dx = -range; dx <= range; ++dx){
      bool xInBounds = ((x + dx) >= 0) && ((x + dx) < size);
      if (!xInBounds)
	continue;
      for (int dy = -range; dy <= range; ++dy){
	bool yInBounds = ((y + dy) >= 0) && ((y + dy) < size);
	if (!yInBounds)
	  continue;
	if ((dx == 0) && (dy == 0))
	  continue;
	neighbors.insert({x + dx, y + dy});
      }      
    }
  }

  template void printSet<int>(const std::set<int> &mySet);
  template void setUnion<int>(const std::set<int> &setA, const std::set<int> &setB, std::set<int> &result);
  template void setDiff<int>(const std::set<int> &in, const std::set<int> &blacklist, std::set<int> &out);
  template bool isSubset<int>(const std::set<int> &small, const std::set<int> &big);
  template void setIntersect<int>(const std::set<int> &setA, const std::set<int> &setB, std::set<int> &result);
}
