#pragma once

#include "PlayerBase.hh"
#include "RandomPlayer.hh"
#include "DeterministicPlayer.hh"

namespace MineSweeper{

  enum class PlayerID {RANDOM, DETERMINISTIC};
  PlayerBase* playerFactory(PlayerID playerID, int size, int nMines);
}
