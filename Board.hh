#pragma once

#include <vector>
#include <utility>

namespace MineSweeper{
  struct ExploredSquare{
  public:
    ExploredSquare(std::pair<int,int> _pt, int _nNeighbors) :
      pt(_pt),
      nNeighbors(_nNeighbors){}
    std::pair<int,int> pt;    
    int nNeighbors;
  };
  
  class Board{
  public:
    Board(int _size, const std::vector<std::pair<int,int>> &_mines); // size up to 50
    void print(bool displayUnexplored);
    bool isSolved();
    void explore(const std::pair<int,int> &move, std::vector<ExploredSquare> &outExploredSquares);
    
  private:
    void initIsExplored();
    void initMines(const std::vector<std::pair<int,int>> &_mines);
    void initNeighbors(); // must be called only after initMines
    
    const int m_size;
    const int m_nMines;
    std::vector<std::vector<bool>> m_isExplored;
    std::vector<std::vector<bool>> m_hasMine;
    std::vector<std::vector<int>> m_nNeighbors;    
  };
}
