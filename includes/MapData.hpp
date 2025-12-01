#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>

class MapData {
public:
  MapData();
  void SetCell(sf::Vector2u cell, char val);
  void Draw(sf::RenderWindow *window);
  sf::Vector2u size;
  std::vector<std::vector<char>> map;
  int flr_r;
  int flr_g;
  int flr_b;
  int sky_r;
  int sky_g;
  int sky_b;
  std::string txt_n;
  std::string txt_s;
  std::string txt_e;
  std::string txt_w;
};

MapData::MapData() {
  this->size.x = 20;
  this->size.y = 20;
  this->map.resize(size.y);
  for (int i = 0; i < this->size.y; i++) {
	this->map[i].resize(size.x);
	for (int j = 0; j < this->size.x; j++)
		this->map[i][j] = ' ';
  }
  this->flr_r = 0;
  this->flr_g = 0;
  this->flr_b = 0;
  this->sky_r = 0;
  this->sky_g = 0;
  this->sky_b = 0;
  this->txt_n = "north texture path";
  this->txt_s = "south texture path";
  this->txt_e = "east texture path";
  this->txt_w = "west texture path";
}

void MapData::SetCell(sf::Vector2u cell, char val){
	this->map[cell.y][cell.x] = val;
}

void MapData::Draw(sf::RenderWindow *window) {
	bool		enemy;
	sf::Font	font("/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-Medium.ttf");
	sf::Text	text(font);
	text.setFillColor(sf::Color::Red);
	sf::Color walls;
	walls.r = 165;
	walls.g = 42;
	walls.b = 42;
	sf::Vector2u scr_sz = window->getSize();
	unsigned int tile_sz = ((scr_sz.y * 0.95) / this->size.y);
	text.setCharacterSize(tile_sz - 2);
	sf::Vector2u start = sf::Vector2u((scr_sz.x - scr_sz.y) * 0.5, (scr_sz.y * 0.05) * 0.5);
	sf::RectangleShape tile({tile_sz, tile_sz});

  for (int i = 0; i < this->size.y; i++) {
	this->map[i].resize(size.x);
	for (int j = 0; j < this->size.x; j++) {
		switch (this->map[i][j]) {
			case '0': {tile.setFillColor(sf::Color::Blue); enemy = false; break ;}
			case '1': {tile.setFillColor(walls); enemy = false; break ;}
			case '2': {tile.setFillColor(sf::Color::Cyan); enemy = false; break ;}
			case 'Z': {tile.setFillColor(sf::Color::Blue); text.setString("E"); enemy = true; break ;}
			case 'B': {text.setString("B"); enemy = true; break ;}
			default: {tile.setFillColor(sf::Color::Green); enemy = false; break ;}
		}
		tile.setPosition({start.x + i * tile_sz, start.y + j * tile_sz});
		window->draw(tile);
		if (enemy) {
			text.setPosition({start.x + i * tile_sz, start.y + j * tile_sz});
			window->draw(text);
		}
	}
  }
}

#endif
