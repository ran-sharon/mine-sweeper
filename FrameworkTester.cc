#include <iostream>
#include "MultiGameFramework.hh"
#include "PlayerBase.hh"

int main(){
  MineSweeper::MultiGameFramework framework(10000,10,1,MineSweeper::PlayerID::RANDOM,false);
  std::cout << framework.runGames() << std::endl;
  return 0;
}
