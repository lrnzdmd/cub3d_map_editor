#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>

class MapData {
public:
    MapData();

    void SetCell(sf::Vector2u cell, char val);
    void Draw(sf::RenderWindow *window, sf::Text text);
    void Resize(sf::Vector2u amt);
    sf::Vector2i GetCellMouse(sf::Vector2i mouse_p, sf::Vector2u win_sz);

    sf::Vector2u size;
    std::vector<std::vector<char>> map;
    
    int flr_r, flr_g, flr_b;
    int sky_r, sky_g, sky_b;

    std::string filename;
    std::string txt_n;
    std::string txt_s;
    std::string txt_e;
    std::string txt_w;
};

#endif