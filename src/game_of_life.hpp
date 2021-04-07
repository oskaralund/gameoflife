#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>

#include "object.hpp"
#include "individual.hpp"
#include "renderer.hpp"

class AntFoodScent;
class AntFood;
struct Tile {
  int type;
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
  double dx() const;
  double dy() const;
  const Tile& GetTile(int, int) const;

private:
  int num_cols_{10};
  int num_rows_{10};
  double dx_{2.0/100.0};
  double dy_{2.0/100.0};
  double dt_{0.01};
  double time_accumulator_{0.0};
  std::vector<std::unique_ptr<Individual>> individuals_;
  std::vector<std::vector<Tile>> tiles_;

  friend Renderer;
  friend Individual;
};

#endif
