/* A GameOfLife object is a variant of Conway's game of life with continuous
 * elements. It discretizes the square [-1,1]x[-1,1] into a grid of Tile
 * objects. On this grid, so-called Agents move around continuously, and
 * react as they transition between Tiles.
*/

#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include "tile.hpp"
#include "agent.hpp"


class GameOfLife {
public:
  using TileIndex = std::array<int, 2>;

  GameOfLife(int rows, int cols);
  void Move(float elapsed_time);
  void AddAgent(std::unique_ptr<Agent>);
  void SetTileType(int i, int j, int type);
  Tile* GetTile(int, int);
  Agent* GetAgent(uint32_t id);
  void RemoveAgent(uint32_t id);
  glm::vec2 GetTileCenter(int, int) const;
  TileIndex PositionToTile(glm::vec2) const;

  int num_rows() const;
  int num_cols() const;
  float dx() const;
  float dy() const;
  float time_factor() const;
  int num_agents() const;

  void set_time_factor(float);

private:
  int num_rows_{100};
  int num_cols_{100};
  float dx_{2.0f/100.0f};
  float dy_{2.0f/100.0f};
  float dt_{1.0f/60.0f};
  float time_accumulator_{0.0f};
  float time_factor_{1.0f};
  std::vector<std::unique_ptr<Agent>> agents_;
  std::vector<std::vector<Tile>> tiles_;

  void Initialize();
  void UpdateTiles(float dt);
};

#endif
