#include <iostream>
#include <utility>
#include <vector>
#include "Board.hh"
#include "PlayerFactory.hh"

using namespace MineSweeper;
using namespace std;

enum class TestLayout {corner};

constexpr int boardSize = 5;

void getMinesLayout(TestLayout testLayout, vector<pair<int,int>> &mines){
  mines.clear();
  if (testLayout == TestLayout::corner){
    mines.push_back({1,1});
  }
}

void testLayout(TestLayout testLayout, PlayerID playerID){
  vector<pair<int,int>> mines;
  getMinesLayout(testLayout, mines);
  unique_ptr<PlayerBase> player(playerFactory(playerID, boardSize, mines.size()));
  unique_ptr<Board> board(new Board(boardSize,mines));
  player->init();
  bool win = true;
  while(!(board->isSolved())){
    board->print(false);
    auto move = player->makeMove();
    vector<ExploredSquare> exploredSquares;
    board->explore(move,exploredSquares);
    if (exploredSquares.size() == 0){
      win = false;      
      break;
    }
    player->postMoveProcess(exploredSquares);
  }   
  board->print(false);
  if (win){
    cout << "Great Success!" << endl;
  } else {
    cout << "Game OVER" << endl;
  }

}

int main(){
  testLayout(TestLayout::corner,PlayerID::DETERMINISTIC);
  return 0;
}
