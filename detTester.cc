#include <iostream>
#include "MultiGameFramework.hh"

int main(){
  MineSweeper::MultiGameFramework framework(1000,10,5,MineSweeper::PlayerID::DETERMINISTIC,false);
  std::cout << framework.runGames() << std::endl;
  return 0;
}
