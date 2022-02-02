#ifndef ANT_CONTROLLER_HPP_
#define ANT_CONTROLLER_HPP_

#include "controller.hpp"

namespace ants {


class AntController : public Controller {
public:
  using Controller::Controller;

private:
  void Paint() const override;
};


} // namespace ants


#endif
