#include "adjacent_tiles.hpp"

#include "game_of_life.hpp"

AdjacentTiles::AdjacentTilesIterator::AdjacentTilesIterator(GameOfLife* game, int i, int j)
  : game_(game)
  , center_i_(i)
  , center_j_(j)
{
  max_row_ = glm::min(i+1, game->GetNumRows()-1);
  min_row_ = glm::max(i-1, 0);
  max_col_ = glm::min(j+1, game->GetNumRows()-1);
  min_col_ = glm::max(j-1, 0);
  i_ = min_row_;
  j_ = min_col_;
  ptr_ = game->GetTile(i_, j_);
}

AdjacentTiles::AdjacentTilesIterator::AdjacentTilesIterator(Tile* tile)
  : ptr_(tile)
  , center_i_(tile->row)
  , center_j_(tile->col)
{}

AdjacentTiles::AdjacentTilesIterator& AdjacentTiles::AdjacentTilesIterator::operator++()
{
  if (j_ == max_col_ && i_ == max_row_)
  {
    ptr_ = game_->GetTile(center_i_, center_j_);
  }

  if (j_ == max_col_)
  {
    j_ = min_col_;
    ++i_;
  }
  else
  {
    ++j_;
  }

  if (i_ == center_i_ && j_ == center_j_)
  {
    if (j_ == max_col_)
    {
      j_ = min_col_;
      ++i_;
    }
    else
    {
      ++j_;
    }
  }

  ptr_ = game_->GetTile(i_, j_);

  return *this;
}

AdjacentTiles::AdjacentTilesIterator AdjacentTiles::AdjacentTilesIterator::operator++(int)
{
  AdjacentTilesIterator tmp = *this;
  ++(*this);
  return tmp;
}

