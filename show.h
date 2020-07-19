#ifndef SHOW_H
#define SHOW_H

#include "base/timer.h"
#include "base/object.h"
#include <SDL.h>

class Show: public GameObject
{
public:
    static int instance;

    Show(int, int, bool appear = true);
    ~Show();
    void update();
    bool is_out();
    void draw(SDL_Surface*);

    bool desappear;

private:
    SDL_Surface* bnw_screenshot;
    Timer timer;
    int cur_image;
};

#endif
