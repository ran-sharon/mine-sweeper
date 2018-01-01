#include <set>
#include <iostream>
#include "Utils.hh"

int main(){
  std::set<int> setA({1,2,3,4,5,6});
  std::set<int> setB({1,3,5,7,9});
  std::set<int> setC({1,3,5});
  Utils::setIntersect(setA,setB,setC);
  //std::cout << Utils::isSubset(setA,setB) << std::endl;
  Utils::printSet(setC);
  return 0;
}
