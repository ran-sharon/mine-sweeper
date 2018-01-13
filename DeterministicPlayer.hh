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
      PointGroup(std::pair<int,int> center, int nMines, int size);
      std::set<std::pair<int,int>> m_members;
      int m_nMines;
    };
    
  public:
    DeterministicPlayer(int size, int nMines);
    virtual std::pair<int,int> makeMove() override;
    virtual void postMoveProcess(const std::vector<ExploredSquare> &exploredPoints) override;
    virtual void init() override;
  private:
    std::set<std::pair<int,int>> m_restOfTheWorld;
    std::set<std::pair<int,int>> m_exploredPoints;
    std::set<std::pair<int,int>> m_mines;
    std::set<std::pair<int,int>> m_dirtyGroups;
    std::set<std::pair<int,int>> m_safePointsForExploration;
    std::map<std::pair<int,int>, PointGroup> m_knownUniverse;
    std::random_device m_rd;
    std::mt19937 m_rng;
    int m_nInitialMines;
    void clearMine(std::pair<int,int> mine);
    void refineGroups(std::pair<int,int> idxA, std::pair<int,int> idxB);
    std::pair<int,int> randomizeRestOfWorldPoint(); // pick one far away from the action.
    void removeSmallGroupFromLarge(std::pair<int,int> idxSmall, std::pair<int,int> idxBig);
    void checkIfGroupIsDone(std::pair<int,int> idx);
    void registerGroupForSafeExploration(std::pair<int,int> idx);
    void registerAsAllMines(std::pair<int,int> idx);
    void removePointFromNeighbors(std::pair<int,int> point, bool isMine);
    void cleanDirtyGroups();
    std::pair<int,int> getBestProbablisticPoint();
    std::pair<int,int> randomizeFromGroup(PointGroup &group);
    double getRestOfWorldMineProbability();
    void removeMinesFromNewGroup(PointGroup &group);
    void removeExploredPointsFromNewGroup(PointGroup &group);
    void removeGroupFromRestOfWorld(PointGroup &group);
  };
}
