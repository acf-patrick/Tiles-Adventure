#ifndef GAME_H
#define GAME_H

#include "base/group.h"
#include "base/timer.h"
#include "base/text.h"
#include "base/app.h"
#include "background.h"
#include "level.h"

class Game : public App
{
public:
    Game();

private:
    void manage_events();
    void update_events();
    void draw();
    void update();
    Sprite* create_enemy(const std::string&);
    Sprite* create_item(const std::string&, bool icon = false);

    Text indicateur;
    Level map;
    Group sprites;
    Background *bg;
    int cur_enemy, cur_item;

    // true: enemy, false: item
    bool focus;
    Text *headers[2];
    Sprite* item;
    Chrono bg_timer;
};

#endif // GAME_H
