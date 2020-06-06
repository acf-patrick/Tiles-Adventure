#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <ctime>
#include "base/func_tool.h"
#include "bubbles.h"

Bubbles::Bubbles(SDL_Rect* v, const SDL_Rect& fan_rect, bool up, int min_tar, int max_tar):
    m_up(up), viewport(v)
{
    type.push_back("Bubble");
    l_bound = fan_rect.x+0.5*fan_rect.w - 22;
    r_bound = fan_rect.x+0.5*fan_rect.w + 8;
    x = randint(l_bound, r_bound);
    x_vel = 0.5;

    int s = (up?-1:1);
    y_vel = s*1.5;
    acc = s*0.05;

    image = rotozoomSurface(IMG_Load("images/Other/Dust Particle.png"), 0, 10.0/(randint(15, 25)), true);
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
    rect.w = rect.h = image->h;
    if (up)
        y = fan_rect.y - rect.h;
    else
        y = fan_rect.y + fan_rect.h;
    y_tar = y+s*randint(min_tar, max_tar);
}

void Bubbles::update()
{
    if ((m_up and y<=y_tar) or (!m_up and y>=y_tar))
        kill();
    if ((x_vel<0 and x<l_bound) or
        (x_vel>0 and x>r_bound))
        x_vel *= -1;
    x += x_vel;
    y += y_vel;
    y_vel += acc;
}

void Bubbles::draw(SDL_Surface* screen)
{
    rect.x = Sint16(x-viewport->x);
    rect.y = Sint16(y-viewport->y);
    SDL_BlitSurface(image, NULL, screen, &rect);
}

bool Bubbles::collide_with(Sprite* sprite)
{
    if (!m_up)
        return false;
    return Sprite::collide_with(sprite);
}
