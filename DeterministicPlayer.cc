#include "DeterministicPlayer.hh"

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
      return false;
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
    auto &group = m_knownUniverse.find(idxBig)->second;
    if (group.m_nMines == 0){
      registerGroupForSafeExploration(idx);
    }
    if (group.m_nMines == group.m_members.size()){
      registerAsAllMines(idx);
    }
  }

  void DeterministicPlayer::registerAsAllMines(pair<int,int> idx){
    auto itGroup = m_knownUniverse.find(idx).second;
    auto points = itGroup->m_members;
    m_knownUniverse.erase(itGroup);
    for (auto point : points){
      m_mines.insert(point);
      removePointFromNeighbors(point, true);
    }        
  }  

  void DeterministicPlayer::registerGroupForSafeExploration(pair<int,int> idx){
    auto itGroup = m_knownUniverse.find(idx).second;
    auto points = itGroup->m_members;
    m_knownUniverse.erase(itGroup);
    for (auto point : points){
      m_safePointsForExploration.insert(point);
      removePointFromNeighbors(point, false);
    }        
  }

  void DeterministicPlayer::removePointFromNeighbors(pair<int,int> point, bool isMine){
    set<pair<int,int>> neighbors;
    Utils::getNeighbors(point,1,neighbors,m_size);
    for (auto &neighbor : neighbors){
      auto &neighborGroupPair = m_knownUniverse.find(neighbor);
      if (neighborGroupPair != m_knownUniverse.end()){
	auto &neighborGroup = neighborGroupPair.second;
	if (neighborGroup->m_members.find(point) != neighborGroup->m_members.end()){
	  if (isMine){
	    neighborGroup->m_nMines--;
	  }
	  neighborGroup->m_members.erase(point);
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
      Utils::getNeighbors(point,2,neighbors,m_size);
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
      double groupProbability = static_cast<double>(group.m_nMines)/static_cast<double>(group.m_members->size());
      if (groupProbability < lowestProbabilityPoint){
	lowestProbabilityPoint = randomizeFromGroup(group);
	lowestProbabilitySoFar = groupProbability;
      }
    }
    
    double restOfWorldProbability = getRestOfWorldMineProbability();
    if (restOfWorldProbability < lowestProbabilitySoFar)
      lowestProbabilityPoint  = randomizeRestOfWorldPoint();
    removePointFromNeighbors(lowestProbabilityPoint);
    return lowestProbabilityPoint;
  }

  double DeterministicPlayer::getRestOfWorldMineProbability(){
    // currently implemented by assuming mines are evenly distributed on the board,
    // can be estimated more accurately with monte-carlo methods
    double restOfWorldPortion = static_cast<double>(m_restOfTheWorld.size())/static_cast<double>(m_size*m_size);
    return restOfWorldPortion * static_cast<double> m_nInitialMines;
  }

  void DeterministicPlayer::postMoveProcess(const vector<ExploredSquare> &exploredPoints){
    for (auto exploredSquare : exploredPoints){
      pair<int,int> pt = {exploredSquare.i, exploredSquare.j};
      PointGroup pointGroup(pt, exploredSquare.nNeighbors);
      removeMinesFromNewGroup(pointGroup);
      removeExploredPointsFromNewGroup(pointGroup);
      m_knownUniverse.insert({pt, pointGroup});
      checkIfGroupIsDone(pt);
      m_dirtyGroups.insert(pt);
    }
    cleanDirtyGroups();
  }

  void DeterministicPlayer::removeMinesFromNewGroup(PointGroup &group){
    group.m_nMines -= Utils::setIntersect(group.m_members, m_mines).size();
    Utils::setDiff(group.m_members, m_mines);
  }

  void DeterministicPlayer::removeExploredPointsFromNewGroup(PointGroup &group){
    Utils::setDiff(group.m_members, m_exploredPoints);
    Utils::setDiff(group.m_members, m_safePointsForExploration);
  }
}
