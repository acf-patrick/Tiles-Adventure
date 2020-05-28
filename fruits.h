#ifndef FRUITS_H
#define FRUITS_H

#include "base/sprite.h"
#include "base/timer.h"
#include <SDL.h>

class Fruit: public Sprite
{
public:
    Fruit(int, int, SDL_Rect*, const std::string&);
    void draw(SDL_Surface*);
    void update();
    void bump(const std::string& flag = "");

private:
    SDL_Rect* viewport;
    bool collected;
    int cur_image;
    Timer timer;
};

#endif // FRUITS_H
