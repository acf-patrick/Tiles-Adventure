#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <ctime>
#include "base/func_tool.h"
#include "bubbles.h"

Bubbles::Bubbles(SDL_Rect* v, int s_x, int s_y):
    viewport(v)
{
    type.push_back("Bubble");
    l_bound = s_x-22;
    r_bound = s_x+8;
    x = randint(l_bound, r_bound);
    y = s_y;
    y_tar = y+randint(100, 130);
    x_vel = 0.5;
    y_vel = 1.5;

    image = rotozoomSurface(IMG_Load("images/Other/Dust Particle.png"), 0, 10.0/(randint(15, 30)), true);
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
    rect.w = rect.h = image->h;
}

void Bubbles::update()
{
    if (y>=y_tar)
        kill();
    if ((x_vel<0 and x<l_bound) or
        (x_vel>0 and x>r_bound))
        x_vel *= -1;
    x += x_vel;
    y += y_vel;
    y_vel += 0.1;
}

void Bubbles::draw(SDL_Surface* screen)
{
    rect.x = Sint16(x-viewport->x);
    rect.y = Sint16(y-viewport->y);
    SDL_BlitSurface(image, NULL, screen, &rect);
}
