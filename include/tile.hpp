#ifndef TILE_HPP_
#define TILE_HPP_

#include <functional>
#include <memory>
#include <array>

/* The Tile is one of the basic types used in the Game of Life.
 * It represents one tile in the grid of tiles on which the Game of Life
 * is taking place. It has six member variables:
 *   row: The tile's row index.
 *   col: The tile's column index.
 *   type: The tile's type.
 *   color: The tile's color in rgba format.
 *   data: An std::share_ptr to optional custom data.
 *   update: An std::function to be called at each time step.
 *
 * As well as one member function:
 *   GetData<T>(): Returns the data pointer cast to T.
*/

struct Tile {
  int row;
  int col;
  int type = 0;
  std::array<uint8_t, 4> color = {0, 0, 0, 255};
  std::shared_ptr<void> data = nullptr;
  std::function<void(Tile*, double)> update = nullptr;

  template<typename T>
  auto GetData()
  {
    return std::static_pointer_cast<T>(data);
  }
};

#endif
