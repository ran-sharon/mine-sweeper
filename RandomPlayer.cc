#include "RandomPlayer.hh"

using namespace std;

namespace MineSweeper{
  RandomPlayer::RandomPlayer(int size, int nMines) :
    PlayerBase(size, nMines),
    m_unexploredPoints(),
    m_rd(),
    m_rng(m_rd()) {}
  
  void RandomPlayer::init(){
    m_unexploredPoints.clear();
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_unexploredPoints.insert({i,j});
      }
    }
  }

  bool RandomPlayer::determineIfSolved(){
    return (static_cast<int>(m_unexploredPoints.size()) <= (m_size*m_size - m_nMines));
  }

  void RandomPlayer::postMoveProcess(const vector<ExploredSquare> &exploredPoints){
    for (auto it = exploredPoints.cbegin(); it != exploredPoints.cend(); ++it){
      auto pt = m_unexploredPoints.find({it->i, it->j});
      if (pt != m_unexploredPoints.end())
	m_unexploredPoints.erase(pt);
    }
  }

  pair<int,int> RandomPlayer::makeMove(){
    uniform_int_distribution<int> uni(0,m_unexploredPoints.size() - 1);
    int idx = uni(m_rng);
    auto it = m_unexploredPoints.begin();
    advance(it, idx);
    return *it;
  }
}
