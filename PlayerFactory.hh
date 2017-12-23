#pragma once

#include "PlayerBase.hh"
#include "RandomPlayer.hh"

namespace MineSweeper{

  enum class PlayerID {RANDOM};
  PlayerBase* playerFactory(PlayerID playerID, int size, int nMines);
}
