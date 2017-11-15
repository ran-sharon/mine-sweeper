#pragma once

#include <vector>
#include <pair>

namespace MineSweeper{
  struct ExploredSquare{
  public:
    ExploredSquare(int _i. int _j, int _nNeighbors) :
      i(_i),
      j(_j),
      nNeighbors(_nNeighbors){}
    int i;
    int j;
    int nNeighbors;
  };
  
  class Board{
  public:
    Board(int _size, int _nMines);
    Board(int _size, const std::vector<std::pair<int,int>> &_mines); // size up to 50
    ~Board();
    void print(bool displayUnexplored);
    bool isSolved();
    std::vector<ExploredSquare> explore(int i, int j);
    
  private:
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
