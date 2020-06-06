#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <ctime>
#include "base/func_tool.h"
#include "bubbles.h"

Bubbles::Bubbles(SDL_Rect* v, const SDL_Rect& fan_rect, int sens, int min_tar, int max_tar):
    m_sens(sens), viewport(v)
{
    int s = ((sens==DROITE or sens==BAS)?1:-1);
    type.push_back("Bubble");

    image = rotozoomSurface(IMG_Load("images/Other/Dust Particle.png"), 0, 10.0/(randint(15, 25)), true);
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
    rect.w = rect.h = image->h;

    acc = s*0.05;
    // vertical
    if (sens==HAUT or sens==BAS)
    {
        if (sens)
            y = fan_rect.y - rect.h;
        else
            y = fan_rect.y + fan_rect.h;
        x_vel = 0.5;
        y_vel = s*1.5;
        l_bound = fan_rect.x;
        u_bound = fan_rect.x+fan_rect.w - rect.w;
        x = randint(l_bound, u_bound);
        target = y+s*randint(min_tar, max_tar);
    }
    // horizontal
    else if (sens==GAUCHE or sens==DROITE)
    {
        if (sens)
            x = fan_rect.x+fan_rect.w;
        else
            x = fan_rect.x-rect.w;
        x_vel = s*1.5;
        y_vel = 0.5;
        l_bound = fan_rect.y;
        u_bound = fan_rect.y+fan_rect.h - rect.h;
        y = randint(l_bound, u_bound);
        target = x+s*randint(min_tar, max_tar);
    }
}

void Bubbles::update()
{
    if ( (m_sens==HAUT and y<=target) or (m_sens==BAS and y>=target) or
        (m_sens==GAUCHE and x<=target) or (m_sens==DROITE and y>=target) )
        kill();
    // vertical
    if (m_sens==HAUT or m_sens==BAS)
    {
        if ((x_vel<0 and x<l_bound) or (x_vel>0 and get_right()>u_bound))
            x_vel *= -1;
    }
    // horizontal
    else if (m_sens==DROITE or m_sens==GAUCHE)
    {
        if ((y_vel<0 and y<l_bound) or (y_vel>0 and get_bottom()>u_bound))
            y_vel *= -1;
    }
    x += x_vel;
    y += y_vel;
    if (m_sens==DROITE or m_sens==GAUCHE)
        x_vel += acc;
    else if (m_sens==HAUT or m_sens==BAS)
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
    if (m_sens == BAS and 100<=target and target<=130)
        return false;
    return Sprite::collide_with(sprite);
}
