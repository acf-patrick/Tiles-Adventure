#ifndef GAME_H
#define GAME_H

#include "background.h"
#include "base/group.h"
#include "base/timer.h"
#include "base/object.h"
#include "base/app.h"
#include "level.h"

class Game : public App
{
public:
    Game();
    ~Game();
    void change_background();

private:
    void manage_events();
    void update_events();
    void draw();
    void update();

    Level map;
    Group sprites;
    Background *bg;

    GameObject *headers[3];
    GameObject *cursor;
    Chrono bg_timer;

};

#endif // GAME_H
