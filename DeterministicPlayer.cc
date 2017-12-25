#include "DeterministicPlayer.hh"

using namespace std;

namespace MineSweeper{  
  
  DeterministicPlayer::DeterministicPlayer(int size, int nMines) :
    PlayerBase(size, nMines),
    m_restOfTheWorld(),
    m_exploredPoints(),
    m_mines(),
    m_clearPointsToExplore(),
    m_rd(),
    m_rng(m_rd()) {}
  
  void DeterministicPlayer::init(){
    m_restOfTheWorld.clear();
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_restOfTheWorld.insert({i,j});
      }
    }
    random_device m_rd;
    mt19937 m_rng(m_rd());
    m_exploredPoints
  }

  void DeterministicPlayer::getNeighbors(pair<int,int> point, int range, set<pair<int,int>> &neighbors){
    range = abs(range);
    neighbors.clear();
    int x = point.first;
    int y = point.second;
    for (int dx = -range; dx <= range; ++dx){
      bool xInBounds = ((x + dx) >= 0) && ((x + dx) < m_size);
      if (!xInBounds)
	continue;
      for (int dy = -range; dy <= range; ++dy){
	bool yInBounds = ((y + dy) >= 0) && ((y + dy) < m_size);
	if (!yInBounds)
	  continue;
	if ((dx == 0) && (dy == 0))
	  continue;
	neighbors.insert({x + dx, y + dy});
      }      
    }
  }

  void DeterministicPlayer::refineGroups(pair<int,int> idxA, pair<int,int> idxB){
    auto itA = m_knownUniverse.find(idxA);
    auto itB = m_knownUniverse.find(idxB);
    if ((itA == m_knownUniverse.end()) || (itB == m_knownUniverse.end())){
      return false;
    }
    bool aInB = Utils::isSubset(itA->m_members,itB->m_members);
    bool bInA = Utils::isSubset(itB->m_members,itA->m_members);
    if (aInB && bInA){
      m_knownUniverse.erase(idxA);
      return;
    }
    if ((!aInB) && (!bInA)){
      return;
    }
    if ((aInB) && (!bInA)){
      removeSmallGroupFromLarge(idxA, idxB);
      dirtify(idxB);
    }
    if ((!aInB) && (bInA)){
      removeSmallGroupFromLarge(idxB, idxA);
      dirtify(idxA);
    }
  }

  void DeterministicPlayer::removeSmallGroupFromLarge(pair<int,int> idxSmall, pair<int,int> idxBig){
    auto itSmall = m_knownUniverse.find(idxSmall);
    auto itBig = m_knownUniverse.find(idxBig);
    if ((idxSmall == m_knownUniverse.end()) || (idxBig == m_knownUniverse.end())){
      throw logic_error("removeSmallGroupFromLarge: one of the groups not found");
    }
    for (auto it : itSmall->m_members){
      itBig->m_members.erase(*it);	
    }
    itBig->m_nMines -= itSmall->m_nMines;
    if (itBig->m_nMines == 0){
      registerForSafeExploration(idxBig);
    }
  }

  

  
  
}
