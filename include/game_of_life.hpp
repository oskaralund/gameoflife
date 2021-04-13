#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include "tile.hpp"
#include "renderer.hpp"
#include "controller.hpp"
#include "individual.hpp"

class GameOfLife
{
public:
  GameOfLife();
  GameOfLife(int rows, int cols);
  void Move(double elapsed_time);
  void AddAntColony(int num_ants);
  void SetTileType(int i, int j, int type);
  Tile* GetTile(int, int);

  int GetNumRows() const;
  int GetNumCols() const;

  class AdjacentTiles;


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

  void Initialize();
  void UpdateTiles(double dt);
  void PositionToTile(glm::dvec2, int*, int*) const;
  glm::dvec2 GetTileCenter(int i, int j) const;

  friend Renderer;
  friend Controller;
  friend Individual;
};

#endif
