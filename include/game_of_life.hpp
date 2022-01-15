/* A GameOfLife object is a variant of Conway's game of life with continuous
 * elements. It discretizes the square [-1,1]x[-1,1] into a grid of Tile
 * objects. On this grid, so-called Individuals move around continuously, and
 * react as they transition between Tiles.
*/

#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include "tile.hpp"
#include "individual.hpp"

class Controller;
class Renderer;

class GameOfLife
{
public:
  GameOfLife(); // Constructs a 100x100 GameOfLife object.
  GameOfLife(int rows, int cols); // Constructs a rows x cols GameOfLife object.
  void Move(float elapsed_time); // Move forward in time.
  void AddIndividual(std::unique_ptr<Individual>);
  void SetTileType(int i, int j, int type);
  Tile* GetTile(int, int);
  glm::dvec2 GetTileCenter(int i, int j) const;

  int GetNumRows() const;
  int GetNumCols() const;

private:
  int num_rows_{100};
  int num_cols_{100};
  float dx_{2.0f/100.0f};
  float dy_{2.0f/100.0f};
  float dt_{1.0f/60.0f};
  float time_accumulator_{0.0f};
  float time_factor_{1.0f};
  std::vector<std::unique_ptr<Individual>> individuals_;
  std::vector<std::vector<Tile>> tiles_;

  void Initialize();
  void UpdateTiles(float dt);
  void PositionToTile(glm::dvec2, int*, int*) const;

  friend Renderer;
  friend Controller;
  friend Individual;
};

#endif
