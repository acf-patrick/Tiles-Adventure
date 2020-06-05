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

class Falling_platform: public Sprite
{
public:
    Falling_platform(SDL_Rect*, int, int);
    ~Falling_platform();
    void update();
    void draw(SDL_Surface*);

private:
    SDL_Rect* viewport;
    bool off;
    int cur_image;
    int y0;
    float y_vel;
    SDL_Surface *s_off, *s_on;
    Timer timer;
    Group bubbles;
};

#endif // TRAPS_H
