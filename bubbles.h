#ifndef BUBBLES_H
#define BUBBLES_H

#include <SDL.h>
#include "base/sprite.h"

class Bubbles: public Sprite
{
public:
    Bubbles(SDL_Rect*, const SDL_Rect&, bool up, int min_tar, int max_tar);
    void update();
    void draw(SDL_Surface*);
    bool collide_with(Sprite*);

private:
    int m_up;
    SDL_Rect* viewport;
    float x_vel, y_vel, acc;
    int l_bound, r_bound;
    int y_tar;
};

#endif // BUBBLES_H
