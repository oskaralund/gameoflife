#include "ant_renderer.hpp"

#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "ant.hpp"

namespace ants
{


void AntRenderer::DrawTiles()
{
  for (std::size_t i = 0; i < game_->num_rows(); ++i)
  {
    for (std::size_t j = 0; j < game_->num_cols(); ++j)
    {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_rows())*4];

      auto tile = game_->GetTile(i, j);
      auto& tile_type = tile->type;

      sf::Color color;
      auto tile_data = tile->GetData<TileData>();
      if (tile->type == TileType::Basic && tile_data)
      {
        color.r = tile_data->colony_scent*255;
        color.g = 0;
        color.b = tile_data->food_scent*255;
        color.a = 255;
      }
      else
      {
        color.r = colors_[tile_type][0];
        color.g = colors_[tile_type][1];
        color.b = colors_[tile_type][2];
        color.a = colors_[tile_type][3];
      }
      quad[0].color = color;
      quad[1].color = color;
      quad[2].color = color;
      quad[3].color = color;
    }
  }
  window_->draw(grid_va_);
}


} // namespace ants
