#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include "base/group.h"

class Background: public GameObject
{
public:
    Background();
    void update();
    void draw(SDL_Surface*);

private:
    std::vector<int> ys;
};

#endif
