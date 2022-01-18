#ifndef ANT_RENDERER_HPP_
#define ANT_RENDERER_HPP_

#include "renderer.hpp"

namespace ants
{


class AntRenderer : public Renderer
{
public:
  using Renderer::Renderer;
private:
  void DrawTiles() override;
};


} // namespace ants

#endif
