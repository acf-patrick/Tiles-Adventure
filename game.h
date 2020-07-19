#ifndef GAME_H
#define GAME_H

#include "background.h"
#include "base/group.h"
#include "base/timer.h"
#include "base/text.h"
#include "base/app.h"
#include "level.h"

class Game : public App
{
public:
    Game();
    ~Game();
    void change_background();

private:
    class Header: public Text
    {
    public:
        Header(const std::string& content, int _x, int _y, bool* s): Text(content, 255, 255, 255, "fonts/Supercell-Magic_5", NULL, 15, _x, _y), show(s) {}
        void draw(SDL_Surface* screen) { if (*show) Text::draw(screen); }
    private:
        bool *show;
    };
    void manage_events();
    void update_events();
    void draw();
    void update();
    GameObject* create_enemy(const std::string&);
    GameObject* create_item(const std::string&, bool icon = false);
    GameObject* create_trap(const std::string&, bool icon = false);

    Text indicateur;
    Level map;
    Group sprites;
    Background *bg;
    int cur_enemy, cur_item, cur_trap;

    enum { ITEM, ENEMY, TRAP };
    int focus;

    GameObject *headers[3];
    GameObject *cursor;
    Chrono bg_timer;

    bool edit;
    int fan_orientation;
};

#endif // GAME_H
