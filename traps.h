#ifndef TRAPS_H
#define TRAPS_H

#include <SDL.h>
#include "base/sprite.h"
#include "base/timer.h"

class Arrow: public Sprite
{
public:
    Arrow(SDL_Rect*, int, int);
    ~Arrow();
    void update();
    void bump(const std::string& flag = "");
    void draw(SDL_Surface*);

    static void check_image_existence(SDL_Surface*);
    static SDL_Rect* create_static_viewport();

private:
    SDL_Rect* viewport;
    bool used;
    int cur_image;
    SDL_Surface *hit, *idle;
    Timer timer;

};

class Basic_fan: public Sprite
{
public:
    Basic_fan(SDL_Rect*, int, int);
    ~Basic_fan();
    virtual void update();
    void draw(SDL_Surface*);

protected:
    SDL_Rect* viewport;
    bool off, ascendant;
    int cur_image;
    // hauteur des bulles
    int min_tar, max_tar;
    SDL_Surface *s_off, *s_on;
    Timer timer;
    Group bubbles;

    int animation_delay;
    bool extinction, demarrage;
    bool m_static;
};

class Falling_platform: public Basic_fan
{
public:
    Falling_platform(SDL_Rect*, int, int);
    void update();
    void bump(const std::string& flag = "");
    bool collide_with(Sprite*);

private:
    int y0;
    float y_vel, gravity;

    void turn_off();
};

class Fan: public Basic_fan
{
public:
    Fan(SDL_Rect*, int, int);
    void update();

private:
    Chrono switch_timer;
};


#endif // TRAPS_H
