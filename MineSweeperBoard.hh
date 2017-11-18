#pragma once

#include <vector>
#include <utility>

namespace MineSweeper{
  struct ExploredSquare{
  public:
    ExploredSquare(int _i, int _j, int _nNeighbors) :
      i(_i),
      j(_j),
      nNeighbors(_nNeighbors){}
    int i;
    int j;
    int nNeighbors;
  };
  
  class Board{
  public:
    Board(int _size, const std::vector<std::pair<int,int>> &_mines); // size up to 50
    void print(bool displayUnexplored);
    void printDebug();
    bool isSolved();
    void explore(int i, int j, std::vector<ExploredSquare> &outExploredSquares);
    
  private:
    void initIsExplored();
    void initMines(const std::vector<std::pair<int,int>> &_mines);
    void initNeighbors(); // must be called only after initMines
    std::vector<std::pair<int,int>> getNeighbors(int i, int j);
    
    const int m_size;
    const int m_nMines;
    std::vector<std::vector<bool>> m_isExplored;
    std::vector<std::vector<bool>> m_hasMine;
    std::vector<std::vector<int>> m_nNeighbors;    
  };
}
