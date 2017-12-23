#pragma once

#include <random>
#include "Board.hh"
#include "PlayerFactory.hh"

namespace MineSweeper{  
  class MultiGameFramework{
  public:
    MultiGameFramework(int nGames, int size, int nMines, PlayerID playerID, bool bPrint);
    double runGames();
  private:
    int m_nGames;
    int m_size;
    int m_nMines;
    bool m_bPrint;
    PlayerID m_playerID;
    std::random_device m_rd;
    std::mt19937 m_rng;
    std::unique_ptr<PlayerBase> player;
    std::unique_ptr<Board> board;
    void prepareNewGame(const std::vector<std::pair<int,int>> &mines);
    int runOneGame();
    std::vector<std::pair<int,int>> randomizeMines();    
  };
}
