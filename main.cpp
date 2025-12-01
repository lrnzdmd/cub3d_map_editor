#include "./includes/MapData.hpp"
#include "sfmlgui/gui.hpp"
#include "sfmlgui/widgets/button.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <optional>

int main() {
  MapData map;
  sf::RenderWindow window(sf::VideoMode({1280, 720}), "cub3d map editor");
  sf::CircleShape shape(500.f);
  SFGUI::SFMLGUI gui(window, "test");
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      gui.pollEvents(*event);
    }
    gui.Update();
    window.clear();
    map.Draw(&window);
    gui.Draw();
    window.display();
  }
}
