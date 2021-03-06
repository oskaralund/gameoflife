/* The Tile is one of the basic types used in the GameOfLife class.
 * It represents one tile in the grid of tiles on which the simulation
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

  // Users can store an update function to be called at each timestep.
  std::function<void(Tile*, float)> update = nullptr;

  // Users can store arbitrary data through the data pointer.
  std::shared_ptr<void> data = nullptr;

  // In order to retrieve user data, you can use the GetData<T>()
  // function. It will return a shared_ptr to the specified type T.
  template<typename T>
  auto GetData() {
    return std::static_pointer_cast<T>(data);
  }
};

#endif
