#pragma once

#include <set>
#include <vector>
#include <utility>
#include <random>
#include <memory>
#include "PlayerBase.hh"

namespace MineSweeper{

  class RandomPlayer : public PlayerBase {
  public:
    using PlayerBase::PlayerBase;
    virtual std::pair<int,int> makeMove() override;
    virtual bool determineIfSolved() override;
    virtual void postMoveProcess(const std::vector<ExploredSquare> &exploredPoints) override;
    virtual void init() override;
  private:
    std::set<std::pair<int,int>> m_unexploredPoints;
    std::random_device m_rd;
    std::mt19937 m_rng;
  };
}
