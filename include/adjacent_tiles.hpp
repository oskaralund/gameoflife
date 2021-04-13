#ifndef ADJACENT_TILES_HPP_
#define ADJACENT_TILES_HPP_

#include <iterator>

#include "tile.hpp"

class GameOfLife;

class AdjacentTiles
{
public:
  struct AdjacentTilesIterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Tile;
    using pointer           = Tile*;
    using reference         = Tile&;

    AdjacentTilesIterator(GameOfLife*, int, int);
    AdjacentTilesIterator(Tile*);

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }
    AdjacentTilesIterator& operator++();
    AdjacentTilesIterator operator++(int);
    friend bool operator==(const AdjacentTilesIterator& a, const AdjacentTilesIterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!=(const AdjacentTilesIterator& a, const AdjacentTilesIterator& b) { return a.ptr_ != b.ptr_; };

    bool done = false;

  private:
    GameOfLife* game_ = nullptr;
    pointer ptr_ = nullptr;
    int max_row_ = 0;
    int min_row_ = 0;
    int max_col_ = 0;
    int min_col_ = 0;
    int center_i_ = 0;
    int center_j_ = 0;
    int i_ = 0;
    int j_ = 0;
  };

  AdjacentTiles(GameOfLife* game, int i, int j) : game_(game), i_(i), j_(j) {}
  AdjacentTilesIterator begin() { return AdjacentTilesIterator(game_, i_, j_); }
  AdjacentTilesIterator end() { return AdjacentTilesIterator(game_, i_, j_); }

private:
  GameOfLife* game_;
  int i_;
  int j_;
};


#endif
