#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <glm/glm.hpp>

class GameOfLife;
struct Tile;

namespace sf
{
  class RenderWindow;
}


class Individual {
public:
  enum class Type { Basic, Ant };

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
  void SetType(Type);
  void SetCurrentTileType(int) const;
  void SetCurrentTileTimer(double) const;
  void SetCurrentTileColor(uint8_t color[4]) const;
  void SetCurrentTileFade(bool fade) const;
  const Tile& GetCurrentTile() const;

private:
  glm::dvec2 position_{0,0};
  glm::dvec2 velocity_{0,0};
  double radius_{0.01};
  Type type_{Type::Basic};
  GameOfLife* game_;

  void GetCurrentTileCoords(int*, int*) const;
  void EnforcePeriodicity();
};

#endif
