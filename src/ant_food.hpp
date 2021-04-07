#ifndef ANT_FOOD_HPP_
#define ANT_FOOD_HPP_

#include "object.hpp"

class AntFood : public Object
{
public:
  AntFood();
  void SetVisited(bool);
  bool HasBeenVisited() const;

private:
  bool visited_{false};
};

#endif
