#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "adjacent_tiles.hpp"

class GameOfLife;
struct Tile;

namespace sf
{
  class RenderWindow;
}


class Individual {
public:
  enum class Direction { Left = 0, Right, Up, Down, None };

  Individual(GameOfLife*);
  virtual void Move(double dt);
  virtual void ReactToTile();
  virtual void Render(sf::RenderWindow* window) const;
  void SetVelocity(glm::dvec2);
  void SetRadius(double);
  double GetRadius() const;
  glm::dvec2 GetPosition() const;
  glm::dvec2 GetVelocity() const;
  GameOfLife* GetGame() const;

protected:
  void SetCurrentTileType(int) const;
  void SetCurrentTileColor(const std::array<uint8_t, 4>& color) const;
  int GetId() const;
  Tile* GetCurrentTile() const;
  glm::dvec2 GetTileCenter(const Tile&);
  void GoToward(glm::dvec2 target);
  void GetCurrentTileCoords(int*, int*) const;
  AdjacentTiles GetAdjacentTiles() const;

private:
  int id_;
  int view_distance_{1};
  glm::dvec2 position_{0,0};
  glm::dvec2 velocity_{0,0};
  Tile* tile_{nullptr};
  double speed_{0.05};
  double radius_{0.001};
  GameOfLife* game_;
  static int instances;

  void EnforceWalls();
};

#endif
