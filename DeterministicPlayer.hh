#pragma once

#include <set>
#include <map>
#include <vector>
#include <utility>
#include <random>
#include <memory>
#include "PlayerBase.hh"

namespace MineSweeper{

  class DeterministicPlayer : public PlayerBase {
    class PointGroup {
    public:
      PointGroup(std::pair<int,int> center, int nMines);
      std::set<std::pair<int,int>> m_members;
      int m_nMines;
    }
    
  public:
    DeterministicPlayer(int size, int nMines);
    virtual std::pair<int,int> makeMove() override;
    virtual bool determineIfSolved() override;
    virtual void postMoveProcess(const std::vector<ExploredSquare> &exploredPoints) override;
    virtual void init() override;
  private:
    std::set<std::pair<int,int>> m_restOfTheWorld;
    std::set<std::pair<int,int>> m_exploredPoints;
    std::set<std::pair<int,int>> m_mines;
    std::set<std::pair<int,int>> m_clearPointsToExplore;
    std::map<std::pair<int,int>, PointGroup> m_knownUniverse;
    std::random_device m_rd;
    std::mt19937 m_rng;
    void clearMine(std::pair<int,int> mine);
    void getNeighbors(std::pair<int,int> point, int range, std::set<std::pair<int,int>> &neighbors);
    bool refineGroups(std::pair<int,int> idxA, std::pair<int,int> idxB);
    std::pair<int,int> randomizeRestOfWorldPoint(); // pick one far away from the action.
  };
}
