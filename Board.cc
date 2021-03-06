#include <stdexcept>
#include <iostream>
#include "Board.hh"
#include "Utils.hh"

using namespace std;

namespace MineSweeper{
  Board::Board(int _size, const vector<pair<int,int>> &_mines) :
    m_size(_size),
    m_nMines(_mines.size()),
    m_isExplored(),
    m_hasMine(),
    m_nNeighbors(){
    if ((_size > 50) || (_size < 0))
      throw invalid_argument("board size should be between 0 and 50");
    for (int i = 0; i < m_size; ++i){
      m_isExplored.push_back(vector<bool>(m_size));
      m_hasMine.push_back(vector<bool>(m_size));
      m_nNeighbors.push_back(vector<int>(m_size));
    }
    initIsExplored();
    initMines(_mines);
    initNeighbors();
  }

  void Board::initIsExplored(){
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_isExplored[i][j] = false;
      }
    }
  }
  
  void Board::print(bool displayUnexplored){
    cout << "printing board" << endl;
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	if (displayUnexplored){
	  if (m_hasMine[i][j])
	    cout << 'X';
	  else
	    cout << m_nNeighbors[i][j];
	}
	else{
	  if (m_isExplored[i][j])
	    cout << m_nNeighbors[i][j];
	  else
	    cout << '-';
	}
      }
      cout << endl;
    }
    cout << endl;
  }
  
  bool Board::isSolved(){
    int nExplored = 0;
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	nExplored += m_isExplored[i][j];
      }
    }
    int nSafeSquares = m_size * m_size - m_nMines;
    return nExplored == nSafeSquares;
  }

  void Board::initMines(const vector<pair<int,int>> &_mines){
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_hasMine[i][j] = false;
      }
    }
    for (auto mine = _mines.cbegin(); mine != _mines.cend(); ++mine){
      m_hasMine[mine->first][mine->second] = true;
    }    
  }
  
  void Board::initNeighbors(){
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	m_nNeighbors[i][j] = 0;
	set<pair<int,int>> neighbors;
	Utils::getNeighbors({i,j}, 1, neighbors, m_size);
	for (auto neighbor : neighbors){
	  m_nNeighbors[i][j] += m_hasMine[neighbor.first][neighbor.second];
	}
      }
    }
  }

  void Board::explore(const pair<int,int> &move, vector<ExploredSquare> &outExploredSquares){
    outExploredSquares.clear();
    vector<pair<int,int>> exploreQueue;
    exploreQueue.push_back(move);
    while (exploreQueue.size() != 0){
      pair<int,int> curPoint = exploreQueue.back();
      int i = curPoint.first;
      int j = curPoint.second;
      exploreQueue.pop_back();
      bool hasMine = m_hasMine[i][j];
      bool isExplored = m_isExplored[i][j];
      if (!hasMine && !isExplored){
	outExploredSquares.push_back(ExploredSquare(curPoint, m_nNeighbors[i][j]));
	m_isExplored[i][j] = true;
	if (m_nNeighbors[i][j] == 0){
	  set<pair<int,int>> neighbors;
	  Utils::getNeighbors({i,j}, 1, neighbors, m_size);
	  for (auto neighbor : neighbors){
	    exploreQueue.push_back(neighbor);
	  }
	}
      }
    }
  }
}
