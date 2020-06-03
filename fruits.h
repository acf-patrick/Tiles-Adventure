#ifndef FRUITS_H
#define FRUITS_H

#include "base/with_mass.h"
#include "base/timer.h"
#include "base/map.h"
#include <SDL.h>

class Fruit: public With_mass
{
public:
    Fruit(int, int, Map*, const std::string&, bool hidden = false);
    void draw(SDL_Surface*);
    void update();
    void bump(const std::string& flag = "");

private:
    SDL_Rect* viewport;
    bool collected;
    int cur_image;
    bool static_item;
    Timer timer;
};

#endif // FRUITS_H
