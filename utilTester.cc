#include <set>
#include <iostream>
#include "Utils.hh"

int main(){
  std::set<int> setA({1,2,3,4,5,9});
  std::set<int> setB({1,2,3,4,5,6,7,8});
  std::cout << Utils::isSubset(setA,setB) << std::endl;
  return 0;
}
