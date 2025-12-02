#include "MapData.hpp"
#include <algorithm> // Necessario per std::min

MapData::MapData() {
    this->size.x = 20;
    this->size.y = 20;
    this->map.resize(size.y);
    
    for (int i = 0; i < (int)this->size.y; i++) {
        this->map[i].resize(size.x);
        for (int j = 0; j < (int)this->size.x; j++)
            this->map[i][j] = ' ';
    }

    this->flr_r = 0;
    this->flr_g = 0;
    this->flr_b = 0;
    this->sky_r = 0;
    this->sky_g = 0;
    this->sky_b = 0;

    this->filename = "newmap.cub";
    this->txt_n = "north texture path";
    this->txt_s = "south texture path";
    this->txt_e = "east texture path";
    this->txt_w = "west texture path";
}

void MapData::SetCell(sf::Vector2u cell, char val) {
    if (cell.y < map.size() && cell.x < map[0].size()) {
        this->map[cell.y][cell.x] = val;
    }
}

sf::Vector2i MapData::GetCellMouse(sf::Vector2i mouse_p, sf::Vector2u win_sz) {
    float margin_factor = 0.95f;
    unsigned int tile_sz_x = (unsigned int)((win_sz.x * margin_factor) / this->size.x);
    unsigned int tile_sz_y = (unsigned int)((win_sz.y * margin_factor) / this->size.y);
    unsigned int tile_sz = std::min(tile_sz_x, tile_sz_y);

    if (tile_sz == 0) 
        return {-1, -1};

    float map_width_px = (float)this->size.x * tile_sz;
    float map_height_px = (float)this->size.y * tile_sz;
    
    sf::Vector2f start; 
    start.x = (win_sz.x > map_width_px) ? (win_sz.x - map_width_px) / 2.0f : 0.0f;
    start.y = (win_sz.y > map_height_px) ? (win_sz.y - map_height_px) / 2.0f : 0.0f;

    float relative_x = mouse_p.x - start.x;
    float relative_y = mouse_p.y - start.y;

    int map_j = (int)(relative_x / tile_sz);
    int map_i = (int)(relative_y / tile_sz);

    if (map_j >= 0 && map_j < (int)this->size.x && 
        map_i >= 0 && map_i < (int)this->size.y)
        return {map_j, map_i};

    return {-1, -1};
}

void MapData::Resize(sf::Vector2u amt) {
    if (amt == this->size)
        return;

    amt.x = amt.x < 3 ? 3 : amt.x;
    amt.y = amt.y < 3 ? 3 : amt.y;

    this->map.resize(amt.y);
    for (int i = 0; i < (int)amt.y; i++)
        this->map[i].resize(amt.x, ' ');

    this->size = amt; 
}

void MapData::Draw(sf::RenderWindow *window, sf::Text text) {
    sf::Color walls(165, 42, 42);
    sf::Color floor(200, 200, 200);
    sf::Color space(20, 20, 70);
    sf::Color door(255, 255, 0);
    
    sf::Vector2u scr_sz = window->getSize();
    float margin_factor = 0.95f;
    unsigned int tile_sz_x = (unsigned int)((scr_sz.x * margin_factor) / this->size.x);
    unsigned int tile_sz_y = (unsigned int)((scr_sz.y * margin_factor) / this->size.y);
    unsigned int tile_sz = std::min(tile_sz_x, tile_sz_y);

    if (tile_sz == 0) return; 

    text.setCharacterSize(tile_sz - 2);
    text.setFillColor(sf::Color::Red);

    float map_width_px = (float)this->size.x * tile_sz;
    float map_height_px = (float)this->size.y * tile_sz;
    
    sf::Vector2f start; 
    start.x = (scr_sz.x > map_width_px) ? (scr_sz.x - map_width_px) / 2.0f : 0.0f;
    start.y = (scr_sz.y > map_height_px) ? (scr_sz.y - map_height_px) / 2.0f : 0.0f;

    sf::RectangleShape tile({(float)tile_sz, (float)tile_sz});
    tile.setOutlineColor(sf::Color::Black);
    tile.setOutlineThickness(-1.f);

    for (int i = 0; i < (int)this->size.y; i++) {
        for (int j = 0; j < (int)this->size.x; j++) {
            char cell_type = this->map[i][j];
            switch (cell_type) {
                case '1': tile.setFillColor(walls); break;
                case '0': tile.setFillColor(floor); break;
                case '2': tile.setFillColor(door); break;
                case ' ': default: tile.setFillColor(space); break;
            }

            tile.setPosition({start.x + j * tile_sz, start.y + i * tile_sz});
            window->draw(tile);

            if (cell_type == 'Z' || cell_type == 'B' || cell_type == 'S') {
                tile.setFillColor(floor);
                window->draw(tile);
                text.setPosition({start.x + j * tile_sz, start.y + i * tile_sz});
                text.setString(std::string(1, cell_type));
                window->draw(text);
            }
        }
    }
}