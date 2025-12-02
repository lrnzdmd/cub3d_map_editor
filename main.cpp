#include "./includes/MapData.hpp"
#include "sfmlgui/gui.hpp"
#include "sfmlgui/widgets/button.hpp"
#include "sfmlgui/widgets/inputbox.hpp"
#include "sfmlgui/widgets/listbox.hpp"
#include "sfmlgui/widgets/slider.hpp"
#include "sfmlgui/widgets/slider2.hpp"
#include "sfmlgui/widgets/slider3.hpp"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

enum map_ch { WALL, SPACE, FLOOR, DOOR, ENEMY, BOSS, START };

bool is_valid_char(char c) {
    return std::string("012ZBSNSEW ").find(c) != std::string::npos;
}

bool is_player_start(char c) {
    return std::string("NSEW").find(c) != std::string::npos;
}

bool check_open_borders(const std::vector<std::vector<char>>& map, int x, int y, int w, int h) 
{
    char c = map[y][x];
    if (c == '1' || c == ' ') 
        return true;
    if (x == 0 || x == w - 1 || y == 0 || y == h - 1) 
        return false;
    bool touches_space = (
        map[y][x - 1] == ' ' ||
        map[y][x + 1] == ' ' ||
        map[y - 1][x] == ' ' ||
        map[y + 1][x] == ' '
    );
    return !touches_space;
}

bool ValidateMap(const MapData& map_data) {
    int player_count = 0;
    int h = map_data.map.size();
    if (h == 0) return false;
    int w = map_data.map[0].size();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            char c = map_data.map[y][x];

            if (!is_valid_char(c)) {
                std::cerr << "Errore: invalid char '" << c << "' at (" << x << "," << y << ")\n";
                return false;
            }
            if (is_player_start(c)) {
                player_count++;
            }
            if (!check_open_borders(map_data.map, x, y, w, h)) {
                std::cerr << "Error: Map open at (" << x << "," << y << ")\n";
                return false;
            }
        }
    }

    if (player_count != 1) {
        std::cerr << "Error: Place only one player, placed " << player_count << ")\n";
        return false;
    }

    return true;
}

char get_map_char(int ch){
    switch (ch) {
        case WALL: return '1';
        case SPACE: return ' ';
        case FLOOR: return '0';
        case DOOR: return '2';
        case ENEMY: return 'Z';
        case BOSS: return 'B';
        case START: return 'S';
        default: return ' ';
    }
}

void export_map(MapData& map) {
	std::cout << "Validating map.\n";
    if (!ValidateMap(map)) {
        std::cerr << "EXPORT FAILED: Invalid Map.\n";
        return;
    }
    std::ofstream file(map.filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot create the file\n";
        return;
    }
    file << "NO " << map.txt_n << std::endl;
    file << "SO " << map.txt_s << std::endl;
    file << "WE " << map.txt_w << std::endl;
    file << "EA " << map.txt_e << std::endl;
    file << std::endl;
    file << "F " << map.flr_r << "," << map.flr_g << "," << map.flr_b << std::endl;
    file << "C " << map.sky_r << "," << map.sky_g << "," << map.sky_b << std::endl;
    file << std::endl;
    for (const auto& row : map.map) {
        for (char cell : row) {
            file << cell;
        }
        file << std::endl;
    }
    file.close();
    std::cout << "Map exported successfully to " << map.filename << "\n";
}

struct EditorContext {
    MapData map;
    sf::Vector2u size_sl;
    int curr_ch;
    std::vector<std::string> ch_names;

    SFGUI::InputBox filename, north_txt, south_txt, east_txt, west_txt;
    SFGUI::Slider3<int> flr_sl, sky_sl;
    SFGUI::Slider2<unsigned int> gui_sz_sl;
    SFGUI::Button sz_btn, exp_btn;
    SFGUI::Listbox list;

    EditorContext() : 
        size_sl(20, 20), 
        curr_ch(0),
        ch_names({"Wall", "Space", "Floor", "Door", "Enemy", "Boss", "Player Start"}),
        filename(&map.filename, "File Name"),
        north_txt(&map.txt_n, "N"),
        south_txt(&map.txt_s, "S"),
        east_txt(&map.txt_e, "E"),
        west_txt(&map.txt_w, "W"),
        flr_sl(&map.flr_r, &map.flr_g, &map.flr_b, "Floor Color", 0, 255),
        sky_sl(&map.sky_r, &map.sky_g, &map.sky_b, "Sky Color", 0, 255),
        gui_sz_sl(&size_sl.x, &size_sl.y, "Map Size", 0, 200),
        sz_btn("Apply new size"),
        exp_btn("Export .cub file"),
        list("Cell to draw", &curr_ch, ch_names.data(), 7) 
    {}
};


void InitInputBoxes(SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    gui.Add(ctx.filename);
    gui.Add(ctx.north_txt);
    gui.Add(ctx.south_txt);
    gui.Add(ctx.east_txt);
    gui.Add(ctx.west_txt);
}

void InitSliders(SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    gui.Add(ctx.gui_sz_sl);
	gui.Add(ctx.sz_btn);
    gui.Add(ctx.flr_sl);
    gui.Add(ctx.sky_sl);
}

void InitButtons(SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    gui.Add(ctx.exp_btn);
}

void InitLists(SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    gui.Add(ctx.list);
}

void InitGUI(SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    InitLists(gui, ctx);
    InitSliders(gui, ctx);
    InitInputBoxes(gui, ctx);
    InitButtons(gui, ctx);
}

void HandleMapPainting(sf::RenderWindow& window, SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        bool is_over_gui = (mousePos.x >= gui.pos.x - 50 && mousePos.x < gui.pos.x + 450 &&
                            mousePos.y >= gui.pos.y - 50 && mousePos.y < gui.pos.y + 350);
        
        bool is_inside_window = (mousePos.x >= 0 && mousePos.y >= 0 && 
                                 static_cast<unsigned>(mousePos.x) < window.getSize().x && 
                                 static_cast<unsigned>(mousePos.y) < window.getSize().y);

        if (!is_over_gui && is_inside_window) {
            sf::Vector2i pos = ctx.map.GetCellMouse(mousePos, window.getSize());
            if (pos.x != -1) {
                ctx.map.SetCell({(unsigned)pos.x, (unsigned)pos.y}, get_map_char(ctx.curr_ch));
            }
        }
    }
}

void HandleEvents(sf::RenderWindow& window, SFGUI::SFMLGUI& gui, EditorContext& ctx) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window.close();
        
        gui.pollEvents(*event);
    }

    HandleMapPainting(window, gui, ctx);

    gui.Update();

    if (ctx.sz_btn.isClicked())
        ctx.map.Resize(ctx.size_sl);
    if (ctx.exp_btn.isClicked())
        export_map(ctx.map);
}

void Render(sf::RenderWindow& window, EditorContext& ctx, sf::Text& text, SFGUI::SFMLGUI& gui) {
    window.clear();
    ctx.map.Draw(&window, text);
    gui.Draw();
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "cub3d map editor");
    window.setFramerateLimit(60);

    sf::Font font("/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-Medium.ttf");
    sf::Text text(font);
    
    EditorContext ctx;
    
    SFGUI::SFMLGUI gui(window, "Settings");
    InitGUI(gui, ctx);

    while (window.isOpen()) {
        HandleEvents(window, gui, ctx);
        Render(window, ctx, text, gui);
    }

    return 0;
}