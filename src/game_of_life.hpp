#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>

#include "individual.hpp"
#include "renderer.hpp"

struct Tile {
  int row;
  int col;
  int type;
  double timer_length;
  double timer;
  bool timer_enabled;
  bool fade;
  int owner;
};

class GameOfLife
{
public:
  GameOfLife();
  void Move(double elapsed_time);
  void AddBasicIndividual();
  void AddAnt();
  void AddAntColony();
  void SetTileType(int i, int j, int type);
  const Tile& GetTile(int, int) const;
  double dx() const;
  double dy() const;

private:
  int num_cols_{200};
  int num_rows_{200};
  double dx_{2.0/100.0};
  double dy_{2.0/100.0};
  double dt_{0.01};
  double time_accumulator_{0.0};
  std::vector<std::unique_ptr<Individual>> individuals_;
  std::vector<std::vector<Tile>> tiles_;

  void UpdateTiles(double dt);
  void PositionToTile(glm::dvec2, int*, int*) const;
  glm::dvec2 GetTileCenter(int i, int j) const;

  friend Renderer;
  friend Individual;
};

#endif
