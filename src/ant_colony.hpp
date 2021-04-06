#ifndef ANT_COLONY_HPP_
#define ANT_COLONY_HPP_

#include "object.hpp"

#include <vector>

#include "ant.hpp"

class AntColony : public Object
{
public:
  AntColony();
  void AddAnt(Ant*);

private:
  std::vector<Ant*> ants_;
};

#endif
