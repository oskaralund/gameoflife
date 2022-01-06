#include "console.hpp"

#include <algorithm>

Console::Console(sf::RenderWindow* window, std::string font)
  : window_{window}
{
  font_.loadFromFile(font);

  const auto window_size = window_->getSize();
  height_ = window_size.y/2.0;
  width_ = window_size.x;
}

void Console::Render() const
{
  if (!active_) { return; }

  const auto prev_view = window_->getView();
  const auto window_size = window_->getSize();
  const auto gui_view = sf::View({0.0f, 0.0f, static_cast<float>(window_size.x), static_cast<float>(window_size.y)});
  window_->setView(gui_view);

  RenderBackground();
  RenderHistory();

  sf::Text current_line;
  current_line.setFont(font_);
  current_line.setCharacterSize(24);
  current_line.setFillColor(sf::Color::White);
  current_line.setPosition({0, height_-24});
  current_line.setString(input_string_);
  window_->draw(current_line);


  window_->setView(prev_view);
}

void Console::RenderBackground() const
{
  sf::RectangleShape rect{{width_, height_}};
  rect.setPosition({0, 0});
  rect.setFillColor({150, 150, 150, 255});
  window_->draw(rect);
}

void Console::RenderHistory() const
{
  int i = 1;
  for (const auto& h : history_)
  {
    sf::Text line;
    line.setFont(font_);
    line.setCharacterSize(24);
    line.setFillColor(sf::Color::White);
    line.setPosition({0, height_-font_size_*(i+1)});
    line.setString(h);
    window_->draw(line);
    ++i;
  }
}

void Console::ProcessInput()
{
  if (!active_)
    return;

  sf::Event event;
  while (window_->pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
        window_->close();
        break;

      case sf::Event::KeyPressed:
        KeyPressed(event);
        break;

      case sf::Event::TextEntered:
        TextEntered(event);
        break;

      case sf::Event::MouseButtonPressed:
        break;

      case sf::Event::MouseMoved:
        break;

      case sf::Event::MouseWheelScrolled:
        break;

      case sf::Event::Resized:
        break;

      default:
        break;
    }
  }
}

void Console::KeyPressed(sf::Event event)
{
  switch (event.key.code)
  {
    case -1:
      Toggle();
      break;
    case sf::Keyboard::Enter:
      ProcessCommand(input_string_);
      MoveInputToHistory();
      break;
    case sf::Keyboard::Tab:
      TabComplete();
      break;
    default:
      break;
  }
}

void Console::TextEntered(sf::Event event)
{
  if (event.text.unicode >= 128)
    return;

  if (event.text.unicode == '\t') // Disregard tab
    return;

  if (event.text.unicode == '\r') // Disregard enter
    return;

  if (event.text.unicode == '\b') // Delete character on backspace
  {
    if (input_string_.getSize() > 0)
      input_string_.erase(input_string_.getSize()-1);
    else
      return;
  }
  else // Else add character
  {
    input_string_ += event.text.unicode;
  }
}

void Console::Toggle()
{
  active_ = !active_;
}

bool Console::IsActive() const
{
  return active_;
}

void Console::ProcessCommand(std::string input)
{
  const auto first_space = input.find(" ");
  const auto command = input.substr(0, first_space);
  std::string arg;
  if (first_space != input.npos)
  {
    arg = input.substr(first_space+1, input.npos);
  }
  else
  {
    arg = "";
  }

  auto it = commands_.find(command);
  if (it != commands_.end())
  {
    commands_[command](arg);
  }
  return;
}

void Console::MoveInputToHistory()
{
  history_.push_front(input_string_);
  if (history_.size() > max_history_)
  {
    history_.pop_back();
  }
  input_string_.clear();
}

void Console::AddCommand(
    const std::string& command, std::function<void(std::string)> f)
{
  commands_.emplace(command, f);
}

void Console::TabComplete()
{
  std::string input_string = input_string_;
  auto input_len = input_string.size();
  for (const auto& com : commands_)
  {
    if (input_string.size() >= com.first.size()) continue;

    auto com_substr = com.first.substr(0, input_string.size());
    if (com_substr.compare(input_string_) == 0)
    {
      input_string_ = com.first;
    }
  }
}
