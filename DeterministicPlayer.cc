#include <iostream>
#include "DeterministicPlayer.hh"
#include "Utils.hh"

using namespace std;

namespace MineSweeper{  
  
  DeterministicPlayer::DeterministicPlayer(int size, int nMines) :
    PlayerBase(size, nMines),
    m_restOfTheWorld(),
    m_exploredPoints(),
    m_mines(),
    m_safePointsForExploration(),
    m_rd(),
    m_rng(m_rd()),
    m_nInitialMines(nMines){}
  
  void DeterministicPlayer::init(){
    m_restOfTheWorld.clear();
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_restOfTheWorld.insert({i,j});
      }
    }
    random_device m_rd;
    mt19937 m_rng(m_rd());
  }

  

  void DeterministicPlayer::refineGroups(pair<int,int> idxA, pair<int,int> idxB){
    auto itA = m_knownUniverse.find(idxA);
    auto itB = m_knownUniverse.find(idxB);
    if ((itA == m_knownUniverse.end()) || (itB == m_knownUniverse.end())){
      return;
    }
    bool aInB = Utils::isSubset(itA->second.m_members,itB->second.m_members);
    bool bInA = Utils::isSubset(itB->second.m_members,itA->second.m_members);
    if (aInB && bInA){
      m_knownUniverse.erase(idxA);
      return;
    }
    if ((!aInB) && (!bInA)){
      return;
    }
    if ((aInB) && (!bInA)){
      removeSmallGroupFromLarge(idxA, idxB);
      m_dirtyGroups.insert(idxB);
    }
    if ((!aInB) && (bInA)){
      removeSmallGroupFromLarge(idxB, idxA);
      m_dirtyGroups.insert(idxA);
    }
  }

  void DeterministicPlayer::removeSmallGroupFromLarge(pair<int,int> idxSmall, pair<int,int> idxBig){
    auto itSmall = m_knownUniverse.find(idxSmall);
    auto itBig = m_knownUniverse.find(idxBig);
    auto &smallGroup = itSmall->second;
    auto &bigGroup = itBig->second;
    if ((itSmall == m_knownUniverse.end()) || (itBig == m_knownUniverse.end())){
      throw logic_error("removeSmallGroupFromLarge: one of the groups not found");
    }
    for (auto member : smallGroup.m_members){
      bigGroup.m_members.erase(member);	
    }
    bigGroup.m_nMines -= smallGroup.m_nMines;
    checkIfGroupIsDone(idxBig);
  }

  void DeterministicPlayer::checkIfGroupIsDone(pair<int,int> idx){
    auto &group = m_knownUniverse.find(idx)->second;
    if (group.m_nMines == 0){
      registerGroupForSafeExploration(idx);
    } else {
      if (group.m_nMines == static_cast<int>(group.m_members.size())){
	registerAsAllMines(idx);
      }
    }
  }

  void DeterministicPlayer::registerAsAllMines(pair<int,int> idx){
    auto &group = m_knownUniverse.find(idx)->second;    
    auto points = group.m_members;
    m_knownUniverse.erase(idx);
    for (auto point : points){
      m_mines.insert(point);
      removePointFromNeighbors(point, true);
    }
  }  

  void DeterministicPlayer::registerGroupForSafeExploration(pair<int,int> idx){
    auto &group = m_knownUniverse.find(idx)->second;
    auto points = group.m_members;
    m_knownUniverse.erase(idx);
    for (auto point : points){
      m_safePointsForExploration.insert(point);
      removePointFromNeighbors(point, false);
    }        
  }

  void DeterministicPlayer::removePointFromNeighbors(pair<int,int> point, bool isMine){
    set<pair<int,int>> neighbors;
    Utils::getNeighbors(point,1,neighbors,m_size);
    for (auto &neighbor : neighbors){
      auto neighborGroupPair = m_knownUniverse.find(neighbor);
      if (neighborGroupPair != m_knownUniverse.end()){
	auto &neighborGroup = neighborGroupPair->second;
	if (neighborGroup.m_members.find(point) != neighborGroup.m_members.end()){
	  if (isMine){
	    neighborGroup.m_nMines--;
	  }
	  neighborGroup.m_members.erase(point);
	  m_dirtyGroups.insert(neighbor);
	}
      }
    }
  }

  void DeterministicPlayer::cleanDirtyGroups(){
    while(m_dirtyGroups.size() != 0){
      auto it = m_dirtyGroups.begin();
      if (m_knownUniverse.find(*it) == m_knownUniverse.end()){
	m_dirtyGroups.erase(it);
	continue;
      }
      set<pair<int,int>> neighbors;
      Utils::getNeighbors(*it,2,neighbors,m_size);
      for (auto &neighbor : neighbors){
	if (m_knownUniverse.find(neighbor) != m_knownUniverse.end())
	  refineGroups(*it, neighbor);
      }
      m_dirtyGroups.erase(it);
    }
  }

  pair<int,int> DeterministicPlayer::makeMove(){
    if (m_safePointsForExploration.size() != 0){
      auto it = m_safePointsForExploration.begin();
      auto pt = *it;
      m_safePointsForExploration.erase(it);
      m_exploredPoints.insert(pt);
      return pt;
    }
    else{
      return getBestProbablisticPoint();  
    }
  }

  pair<int,int> DeterministicPlayer::getBestProbablisticPoint(){
    double lowestProbabilitySoFar = 1;
    pair<int,int> lowestProbabilityPoint = *(m_restOfTheWorld.begin()); // place-holder only
    for (auto universeMember : m_knownUniverse){
      auto &group = universeMember.second;
      double groupProbability = static_cast<double>(group.m_nMines)/static_cast<double>(group.m_members.size());
      if (groupProbability < lowestProbabilitySoFar){
	lowestProbabilityPoint = randomizeFromGroup(group);
	lowestProbabilitySoFar = groupProbability;
      }
    }
    
    double restOfWorldProbability = getRestOfWorldMineProbability();
    if ((m_restOfTheWorld.size()!=0) && (restOfWorldProbability < lowestProbabilitySoFar)){
      lowestProbabilityPoint  = randomizeRestOfWorldPoint();
    }      
    removePointFromNeighbors(lowestProbabilityPoint, false);
    return lowestProbabilityPoint;
  }

  pair<int,int> DeterministicPlayer::randomizeRestOfWorldPoint(){
    uniform_int_distribution<int> uni(0,m_restOfTheWorld.size() - 1);
    int idx = uni(m_rng);
    auto it = m_restOfTheWorld.begin();
    advance(it, idx);
    return *it;
  }
  
  pair<int,int> DeterministicPlayer::randomizeFromGroup(PointGroup &group){
    uniform_int_distribution<int> uni(0,group.m_members.size() - 1);
    int idx = uni(m_rng);
    auto it = group.m_members.begin();
    advance(it, idx);
    return *it;
  }

  double DeterministicPlayer::getRestOfWorldMineProbability(){
    // currently implemented by assuming mines are evenly distributed on the board,
    // can be estimated more accurately with monte-carlo methods
    double restOfWorldPortion = static_cast<double>(m_restOfTheWorld.size())/static_cast<double>(m_size*m_size);
    return restOfWorldPortion * static_cast<double> (m_nInitialMines) / static_cast<double>(m_size*m_size);
  }

  void DeterministicPlayer::postMoveProcess(const vector<ExploredSquare> &exploredPoints){
    for (auto exploredSquare : exploredPoints){
      pair<int,int> pt = exploredSquare.pt;
      m_exploredPoints.insert(pt);
      m_safePointsForExploration.erase(pt);
      m_restOfTheWorld.erase(pt);
      removePointFromNeighbors(pt,false);
      PointGroup pointGroup(pt, exploredSquare.nNeighbors, m_size);
      removeMinesFromNewGroup(pointGroup);
      removeExploredPointsFromNewGroup(pointGroup);
      removeGroupFromRestOfWorld(pointGroup);
      m_knownUniverse.insert({pt, pointGroup});
      checkIfGroupIsDone(pt);
      m_dirtyGroups.insert(pt);
    }
    cleanDirtyGroups();
  }

  void DeterministicPlayer::removeGroupFromRestOfWorld(PointGroup &group){
    for (auto pt : group.m_members)
      m_restOfTheWorld.erase(pt);
  }

  void DeterministicPlayer::removeMinesFromNewGroup(PointGroup &group){
    set<pair<int,int>> intersect;
    Utils::setIntersect(group.m_members, m_mines, intersect);
    group.m_nMines -= intersect.size();
    Utils::setDiff(group.m_members, m_mines);
  }

  void DeterministicPlayer::removeExploredPointsFromNewGroup(PointGroup &group){
    Utils::setDiff(group.m_members, m_exploredPoints);
    Utils::setDiff(group.m_members, m_safePointsForExploration);
  }

  DeterministicPlayer::PointGroup::PointGroup(pair<int,int> center, int nMines, int size) :
    m_members(), m_nMines(nMines){
    Utils::getNeighbors(center, 1, m_members, size);
  }
}
