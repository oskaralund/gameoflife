#include "ant_colony.hpp"

AntColony::AntColony()
{
  SetType(Object::Type::AntColony);
}

void AntColony::AddAnt(Ant* ant)
{
  ants_.push_back(ant);
}
