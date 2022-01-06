#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <list>
#include <string>
#include <map>
#include <functional>

#include <SFML/Graphics.hpp>

class Console
{
public:
  Console(sf::RenderWindow* window, std::string font);

  void Render() const;
  void ProcessInput();
  void Toggle();
  bool IsActive() const;
  void AddCommand(const std::string&, std::function<void(std::string)> f);

private:
  void ProcessCommand(std::string);
  void MoveInputToHistory();
  void RenderBackground() const;
  void RenderHistory() const;
  void TabComplete();
  void KeyPressed(sf::Event);
  void TextEntered(sf::Event);

  sf::RenderWindow* window_;
  sf::String input_string_;
  std::list<std::string> history_;
  std::map<std::string, std::function<void(std::string)>> commands_;
  sf::Font font_;
  int font_size_ = 20;
  int max_history_ = 100;
  float height_;
  float width_;
  bool active_ = false;
};

#endif
