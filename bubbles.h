#ifndef BUBBLES_H
#define BUBBLES_H

#include <SDL.h>
#include "base/sprite.h"

class Bubbles: public Sprite
{
public:
    Bubbles(SDL_Rect*, int, int);
    void update();
    void draw(SDL_Surface*);

private:
    SDL_Rect* viewport;
    float x_vel, y_vel;
    int l_bound, r_bound;
    int y_tar;
};

#endif // BUBBLES_H
