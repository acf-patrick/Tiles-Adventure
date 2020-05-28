#ifndef ENEMY_H
#define ENEMY_H

#include "base/with_mass.h"
#include "base/sprite.h"
#include "base/timer.h"
#include "base/group.h"
#include "base/map.h"
#include <vector>
#include <SDL.h>
#include <map>

class Enemy : public With_mass
{
public:
    Enemy(Map* ,int ,int ,const std::string& ,int ,int);
    virtual ~Enemy();
    virtual void bump(const std::string& flag = "");
    virtual void draw(SDL_Surface*);
    virtual SDL_Surface* get_surface();

protected:
    // left and right
    std::map<std::string, SDL_Surface*> images[2];
    std::string state;
    int cur_image;
    int delay;
    Timer timer;
    Timer state_timer;

    void load_images(const std::vector<std::string>&);
    void update_image();
    void stop_falling();
    void wait();
    bool erase();
};

class Shot: public Enemy
{
public:
    Shot(const std::string&, Map*, int, int, int, int, Sprite*, Group*);
    void draw(SDL_Surface*);

protected:
    Sprite* target;
    void fire();

private:
    Group* bullets;
};

#endif
