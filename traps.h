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

private:
    SDL_Rect* viewport;
    bool used;
    int cur_image;
    SDL_Surface *hit, *idle;
    Timer timer;

    void check_image_existence(SDL_Surface*);
};

#endif // TRAPS_H
