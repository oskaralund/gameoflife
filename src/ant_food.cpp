#include "ant_food.hpp"

AntFood::AntFood()
{
  SetType(Object::Type::AntFood);
}

void AntFood::SetVisited(bool visited)
{
  visited_ = visited;
}

bool AntFood::HasBeenVisited() const
{
  return visited_;
}
