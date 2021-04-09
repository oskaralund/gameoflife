#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>
#include <functional>

#include "individual.hpp"
#include "renderer.hpp"
#include "controller.hpp"

struct Tile {
  int row;
  int col;
  int type;
  double timer_length;
  double timer;
  bool timer_enabled;
  bool fade;
  std::shared_ptr<void> data = nullptr;
  std::function<void(double)> update = nullptr;

  template<typename T>
  auto GetData()
  {
    return std::static_pointer_cast<T>(data);
  }
};

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

private:
  int num_rows_{100};
  int num_cols_{100};
  double dx_{2.0/100.0};
  double dy_{2.0/100.0};
  double dt_{0.01};
  double time_accumulator_{0.0};
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
