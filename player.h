#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include "show.h"
#include "base/with_mass.h"
#include "base/map.h"
#include "base/timer.h"
#include "base/sprite.h"

class Player: public With_mass
{
public:
    Player(Map*, int, int, bool*, const std::string&);
    ~Player();
    void update();
    void draw(SDL_Surface*);
    void bump(const std::string& flag = "");
    bool collide_with(Sprite*);
    SDL_Surface* get_surface();

private:
    Show* show;
    std::map<std::string, SDL_Surface*> images[2];
    bool* keys;
    std::string state;
    int cur_image;
    Timer timer;
    bool allow_jump;
    const float JUMP_GRAVITY;
    const float JUMP_IMPULSE;
    const float MAX_FALL_SPEED;

    void respawn();
    void jump();
    void update_states();
    void update_keys();
    void update_xvel();
    void update_yvel();
};

#endif // PLAYER_H
