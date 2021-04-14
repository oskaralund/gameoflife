/* The Tile is one of the basic types used in the Game of Life.
 * It represents one tile in the grid of tiles on which the Game of Life
 * is taking place.
*/

#ifndef TILE_HPP_
#define TILE_HPP_

#include <functional>
#include <memory>
#include <array>

struct Tile {
  int row; // The row index of the tile.
  int col; // The column index of the tile.
  int type = 0; // Each tile has a type which Individuals can use
                // to determine behavior (see individual.hpp).
  std::array<uint8_t, 4> color = {0, 0, 0, 255}; // The color used when rendering the tile.

  // Users can store an update function to be called at each timestep.
  std::function<void(Tile*, double)> update = nullptr;

  // Users can store arbitrary data through the data pointer.
  std::shared_ptr<void> data = nullptr;

  // In order to retrieve user data, you can use the GetData<T>()
  // function. It will return a shared_ptr to the specified type T.
  template<typename T>
  auto GetData()
  {
    return std::static_pointer_cast<T>(data);
  }
};

#endif
