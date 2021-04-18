/* A GameOfLife object stores a grid of Tile objects and a vector
 * of Individual objects. The grid lives on the square [-1,1]x[-1,1].
 * The GameOfLife can step forward in time through the Move function.
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
  void Move(double elapsed_time); // Move forward in time.
  void AddAntColony(int num_ants); // Add an ant colony at the center of the map.
  void SetTileType(int i, int j, int type);
  Tile* GetTile(int, int);

  int GetNumRows() const;
  int GetNumCols() const;

private:
  int num_rows_{100};
  int num_cols_{100};
  double dx_{2.0/100.0};
  double dy_{2.0/100.0};
  double dt_{1.0/60.0};
  double time_accumulator_{0.0};
  double time_factor_{1.0};
  std::vector<std::unique_ptr<Individual>> individuals_;
  std::vector<std::vector<Tile>> tiles_;
  uint8_t colors_[10][4] = {
    {0, 0, 0, 255},
    {0, 255, 0, 255},
    {160, 82, 45, 255},
    {80, 80, 80, 255},
    {0, 255, 255, 255},
    {255, 0, 255, 255},
    {255, 255, 0, 255},
    {100, 0, 0, 255},
    {0, 100, 0, 255},
    {0, 0, 100, 255}
  };


  void Initialize();
  void UpdateTiles(double dt);
  void PositionToTile(glm::dvec2, int*, int*) const;
  glm::dvec2 GetTileCenter(int i, int j) const;

  friend Renderer;
  friend Controller;
  friend Individual;
};

#endif
