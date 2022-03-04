#include "renderer.hpp"

#include "game_of_life.hpp"
#include "ant.hpp"

Renderer::Renderer(sf::RenderWindow* window, GameOfLife* game)
    : window_(window)
    , game_(game)
    , grid_va_(sf::Quads, game->num_cols()*game_->num_rows()*4)
    , individual_va_(sf::Points, game->num_agents())
    , gui_view_{window->getDefaultView()} {
  world_view_.setCenter(sf::Vector2f{0.0f, 0.0f});
  MatchWindowCameraRatio();
  window_->setView(world_view_);
  clock_.restart();
  font_.loadFromFile("../res/arial.ttf");
  header_.setFont(font_);
  header_.setCharacterSize(24);
  header_.setFillColor(sf::Color::White);

  const auto dx = game_->dx();
  const auto dy = game_->dy();
  const auto padding = 0.3;
  for (std::size_t i = 0; i < game_->num_rows(); ++i) {
    for (std::size_t j = 0; j < game_->num_cols(); ++j) {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_rows())*4];
      quad[0].position = sf::Vector2f(-1 + (j+padding)*dx, -1 + (i+padding)*dy);
      quad[1].position = sf::Vector2f(-1 + (j+1-padding)*dx, -1 + (i+padding)*dy);
      quad[2].position = sf::Vector2f(-1 + (j+1-padding)*dx, -1 + (i+1-padding)*dy);
      quad[3].position = sf::Vector2f(-1 + (j+padding)*dx, -1 + (i+1-padding)*dy);

      quad[0].color = sf::Color::Black;
      quad[1].color = sf::Color::Black;
      quad[2].color = sf::Color::Black;
      quad[3].color = sf::Color::Black;
    }
  }
}

void Renderer::Render() {
  window_->setView(world_view_);
  DrawTiles();
  //DrawGrid();
  DrawOuterWalls();
  DrawAgents();

  window_->setView(gui_view_);
  DrawGUI();
}

void Renderer::MoveCamera(sf::Vector2f delta) {
  world_view_.move(delta);
}

void Renderer::Zoom(float factor) {
  world_view_.zoom(factor);
  window_->setView(world_view_);
}

void Renderer::DrawGrid() {
  for (int i = 0; i < game_->num_rows()+1; ++i) {
    const auto y = -1 + 2*i/static_cast<float>(game_->num_rows());
    sf::Vertex line[] =
    {
      sf::Vertex(sf::Vector2f(-1, y)),
      sf::Vertex(sf::Vector2f(1, y))
    };

    window_->draw(line, 2, sf::Lines);
  }

  for (int i = 0; i < game_->num_cols()+1; ++i) {
    const auto x = -1 + 2*i/static_cast<float>(game_->num_cols());
    sf::Vertex line[] =
    {
      sf::Vertex(sf::Vector2f(x, -1)),
      sf::Vertex(sf::Vector2f(x, 1))
    };

    window_->draw(line, 2, sf::Lines);
  }

}

void Renderer::DrawOuterWalls() {
  sf::Vertex lines[] = {
    sf::Vertex(sf::Vector2f(-1, -1)),
    sf::Vertex(sf::Vector2f(1, -1)),
    sf::Vertex(sf::Vector2f(-1, 1)),
    sf::Vertex(sf::Vector2f(1, 1)),
    sf::Vertex(sf::Vector2f(1, -1)),
    sf::Vertex(sf::Vector2f(1, 1)),
    sf::Vertex(sf::Vector2f(-1, -1)),
    sf::Vertex(sf::Vector2f(-1, 1))
  };
  window_->draw(lines, 8, sf::Lines);
}

void Renderer::DrawTiles() {
  for (std::size_t i = 0; i < game_->num_rows(); ++i) {
    for (std::size_t j = 0; j < game_->num_rows(); ++j) {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_rows())*4];

      auto tile = game_->GetTile(i, j);
      auto& tile_type = tile->type;

      sf::Color color;
      color.r = colors_[tile_type%10][0];
      color.g = colors_[tile_type%10][1];
      color.b = colors_[tile_type%10][2];
      color.a = colors_[tile_type%10][3];
      quad[0].color = color;
      quad[1].color = color;
      quad[2].color = color;
      quad[3].color = color;
    }
  }
  window_->draw(grid_va_);
}

void Renderer::DrawAgents() {
  for (std::size_t i = 0; i < game_->num_agents(); ++i) {
    sf::Vertex* point = &individual_va_[i];
    const auto& pos = game_->GetAgent(i)->position();
    point->position = {static_cast<float>(pos[0]), static_cast<float>(pos[1])};
    point->color = {255, 255, 255, 255};
  }
  window_->draw(individual_va_);
}

void Renderer::ZoomAt(sf::Vector2i pixel, float factor) {
  const sf::Vector2f before_coord{
    window_->mapPixelToCoords(pixel, world_view_)};
  world_view_.zoom(factor);
  const sf::Vector2f after_coord{
    window_->mapPixelToCoords(pixel, world_view_)};
  const sf::Vector2f offset_coords{before_coord - after_coord};
  world_view_.move(offset_coords);
}

void Renderer::MatchWindowCameraRatio() {
  const auto window_size = window_->getSize();
  const auto width = static_cast<float>(window_size.x);
  const auto height = static_cast<float>(window_size.y);
  const auto ratio = width/height;
  world_view_.setSize(sf::Vector2f{2.0f*ratio, 2.0f});
}

void Renderer::DrawGUI() {
  DrawBrushSelector();
  DrawBrush();
  DrawHeader();
}

void Renderer::DrawBrushSelector() {
  const auto padding = 10.0f;
  const auto rect_size = 20.0f;
  sf::RectangleShape rect{{rect_size, rect_size}};
  const auto startx = padding;
  const auto starty = window_->getSize().y - padding - 20;

  for (int i = 0; i < 10; ++i) {
    rect.setPosition({startx + i*(rect_size + padding), starty});
    rect.setFillColor({colors_[i][0],
                       colors_[i][1],
                       colors_[i][2],
                       colors_[i][3]});

    if (i == selected_color_) {
      rect.setOutlineThickness(3);
      rect.setOutlineColor(sf::Color(250, 150, 100));
    }
    else {
      rect.setOutlineThickness(0);
    }

    window_->draw(rect);
  }
}

void Renderer::DrawBrush() {
  auto cursor = GetCursorWorldPosition();
  auto [i, j] = game_->PositionToTile({cursor.x, cursor.y});
  int max_i = glm::min(i+1, game_->num_rows()-1);
  int min_i = glm::max(i-1, 0);
  int max_j = glm::min(j+1, game_->num_cols()-1);
  int min_j = glm::max(j-1, 0);


  const auto padding = 0.3f;
  const auto dx = game_->dx();
  const auto dy = game_->dy();

  sf::RectangleShape rect{{dx*(1-2*padding), dy*(1-2*padding)}};
  window_->setView(world_view_);
  for (int i = min_i; i <= max_i; ++i) {
    for (int j = min_j; j <= max_j; ++j) {
      rect.setPosition({-1 + (j+padding)*dy, -1 + (i+padding)*dx});
      rect.setFillColor({colors_[selected_color_][0],
                         colors_[selected_color_][1],
                         colors_[selected_color_][2],
                         colors_[selected_color_][3]});
      window_->draw(rect);
    }
  }
  window_->setView(gui_view_);
}

void Renderer::DrawHeader() {
  const auto time_factor = std::to_string((game_->time_factor()));
  const auto paused = game_->time_factor() == 0 ? "Paused" : "";
  std::string string = time_factor + "x\t" + paused;
  header_.setString(string);
  window_->draw(header_);
}

void Renderer::set_selected_color(int i) {
  selected_color_ = glm::clamp(i, 0, 10);
}

sf::Vector2f Renderer::GetCursorWorldPosition() const {
  auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  auto cursor_world_pos = window_->mapPixelToCoords(
      cursor_pixel_pos, world_view_);

  return cursor_world_pos;
}
