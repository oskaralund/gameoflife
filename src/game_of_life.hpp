#ifndef GAME_OF_LIFE_HPP_
#define GAME_OF_LIFE_HPP_

#include <vector>
#include <memory>

#include "individual.hpp"
#include "object.hpp"
#include "renderer.hpp"

class GameOfLife
{
public:
  void Move(double elapsed_time);
  void AddBasicIndividual();
  void AddAnt();
  void AddAntColony();
  void AddAntFood(glm::dvec2 position);
  std::vector<Object*> GetObjects() const;

private:
  double dt_{0.01};
  double time_accumulator_{0.0};
  std::vector<std::unique_ptr<Individual>> individuals_;
  std::vector<std::unique_ptr<Object>> objects_;

  friend void Renderer::Render();
};

#endif
