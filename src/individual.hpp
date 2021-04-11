#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <vector>

#include <glm/glm.hpp>

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
  void SetCurrentTileColor(uint8_t color[4]) const;
  int GetId() const;
  Tile* GetCurrentTile() const;
  Tile* GetAdjacentTile(Direction) const;
  Direction GetCurrentDirection() const;
  void GetOrthogonalDirections(Direction* a, Direction* b) const;
  glm::dvec2 GetTileCenter(const Tile&);
  glm::dvec2 DirectionToVector(Direction dir) const;
  void GoToward(glm::dvec2 target);
  std::vector<Tile*> GetSurroundingTiles() const;
  void GetCurrentTileCoords(int*, int*) const;

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
