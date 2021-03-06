#include "ant_controller.hpp"

#include "game_of_life.hpp"
#include "ant.hpp"
#include "renderer.hpp"

namespace ants
{


void AntController::Paint() const {
  auto cursor = renderer()->GetCursorWorldPosition();
  auto brush_type = selected_brush_type();
  auto [i, j] = game()->PositionToTile({cursor.x, cursor.y});
  int max_i = glm::min(i+brush_size(), game()->num_rows()-1);
  int min_i = glm::max(i-brush_size(), 0);
  int max_j = glm::min(j+brush_size(), game()->num_cols()-1);
  int min_j = glm::max(j-brush_size(), 0);


  for (int i = min_i; i <= max_i; ++i) {
    for (int j = min_j; j <= max_j; ++j) {
      if (brush_type == TileType::Food) {
        auto tile_data = game()->GetTile(i, j)->GetData<TileData>();
        tile_data->food_scent = 1.0f;
        tile_data->food = 500;
      }
      game()->SetTileType(i, j, brush_type);
    }
  }
}


} // namespace ants
