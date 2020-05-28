#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include "base/group.h"

class Background: public Sprite
{
public:
    Background();
    void update();
    void draw(SDL_Surface*);

private:
    std::vector<int> ys;
};

#endif
