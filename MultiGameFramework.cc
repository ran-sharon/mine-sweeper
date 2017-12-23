#include "MultiGameFramework.hh"
#include <iostream>

using namespace std;

namespace MineSweeper{
  MultiGameFramework::MultiGameFramework(int nGames, int size, int nMines, PlayerID playerID, bool bPrint) :
    m_nGames(nGames),
    m_size(size),
    m_nMines(nMines),
    m_bPrint(bPrint),
    m_playerID(playerID),
    m_rd(),
    m_rng(m_rd()){}

  double MultiGameFramework::runGames(){
    double nWins = 0;
    for (int iGame = 0; iGame < m_nGames; ++iGame){
      nWins += runOneGame();
    }
    return nWins/static_cast<double>(m_nGames);
  }

  void MultiGameFramework::prepareNewGame(const vector<pair<int,int>> &mines){
    player = unique_ptr<PlayerBase>(playerFactory(m_playerID, m_size, m_nMines));
    board = unique_ptr<Board>(new Board(m_size, mines));
  }

  int MultiGameFramework::runOneGame(){
    vector<pair<int,int>> mines = randomizeMines();
    prepareNewGame(mines);
    player->init();
    while(!(board->isSolved())){
      if (m_bPrint)
	board->print(false);
      auto move = player->makeMove();
      vector<ExploredSquare> exploredSquares;
      board->explore(move,exploredSquares);
      if (exploredSquares.size() == 0){
	return 0;
      }
      player->postMoveProcess(exploredSquares);
    }   
    if (m_bPrint)
      board->print(false);
    return 1;
  }

  vector<pair<int,int>> MultiGameFramework::randomizeMines(){
    uniform_int_distribution<int> dist(0, m_size-1);
    vector<vector<bool>> isMine(m_size,vector<bool>(m_size));
    for (int i = 0; i < m_size; ++i){
      for (int j = 0; j < m_size; ++j){
	isMine[i][j] = false;
      }
    }
    vector<pair<int,int>> mines;
    int nRemainingMines = m_nMines;
    if (m_nMines > m_size*m_size){
      throw logic_error("Too much mines");
    }
      
    int nIterations = 0;
    while (nRemainingMines > 0){
      nIterations++;
      if (nIterations > m_nMines * 1000){
	throw runtime_error("Unable to place mines");
      }
      int i = dist(m_rng);
      int j = dist(m_rng);
      if (!isMine[i][j]){
	isMine[i][j] = true;
	mines.push_back({i,j});
	nRemainingMines--;
      }
    }
    return mines;
  }
}
