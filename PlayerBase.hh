#pragma once

#include "Board.hh"

namespace MineSweeper{

  class PlayerBase{
  public:
    PlayerBase(int size, int nMines) : m_size(size), m_nMines(nMines) {}
    virtual std::pair<int,int> makeMove() = 0;
    virtual void postMoveProcess(const std::vector<ExploredSquare> &exploredPoints) = 0;
    virtual void init() {}
  protected:
    int m_size;
    int m_nMines;
  };
}
