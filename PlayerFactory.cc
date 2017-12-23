#include "PlayerFactory.hh"

namespace MineSweeper{

  PlayerBase* playerFactory(PlayerID playerID, int size, int nMines){
    switch (playerID){
    case PlayerID::RANDOM: return new RandomPlayer(size, nMines);
    default: return new RandomPlayer(size, nMines);
    }
  }
}
