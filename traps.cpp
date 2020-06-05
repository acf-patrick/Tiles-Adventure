#include <SDL_image.h>
#include "base/app.h"
#include "traps.h"

Arrow::Arrow(SDL_Rect* v, int _x, int _y):
    viewport(v), used(false),
    cur_image(0)
{
    if (!viewport)
        viewport = create_static_viewport();
    type.push_back("Arrow");
    x = _x;
    y = _y;
    rect.w = rect.h = 18;
    hit = IMG_Load("images/Traps/Arrow/Hit (18x18).png");
    check_image_existence(hit);
    image = idle = IMG_Load("images/Traps/Arrow/Idle (18x18).png");
    check_image_existence(idle);
}

Arrow::~Arrow()
{
    SDL_FreeSurface(idle);
    SDL_FreeSurface(hit);
    image = hit = idle = NULL;
}

void Arrow::update()
{
    image = used?hit:idle;
    if (timer.get_elapsed_ms() >= 100)
    {
        cur_image++;
        timer.restart();
    }
    if (used and cur_image >= 4)
    {
        type.pop_back();
        type.push_back("Arrow");
        kill();
    }
    cur_image %= image->w/rect.w;
}

void Arrow::draw(SDL_Surface* screen)
{
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = { Sint16(x-viewport->x), Sint16(y-viewport->y) };
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Arrow::bump(const std::string& flag)
{
    if (!used)
    {
        cur_image = 0;
        used = true;
        type.pop_back();
        type.push_back("dead arrow");
    }
}

void Arrow::check_image_existence(SDL_Surface* surface)
{
    if (!surface)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
}

SDL_Rect* Arrow::create_static_viewport()
{
    SDL_Rect *ret = new SDL_Rect;
    if (!ret)
        return NULL;
    ret->x = ret->y = 0;
    ret->w = App::width;
    ret->h = App::height;
    return ret;
}

Falling_platform::Falling_platform(SDL_Rect* v, int _x, int _y):
    viewport(v),
    off(false), cur_image(0)
{
    if (!viewport)
    {
        viewport = Arrow::create_static_viewport();
        y_vel = 0;
    }
    else
        y_vel = 0.5;
    type.push_back("Falling Platform");

    x = _x;
    y0 = y = _y;
    s_off = IMG_Load("images/Traps/Falling Platforms/Off.png");
    Arrow::check_image_existence(s_off);
    image = s_on = IMG_Load("images/Traps/Falling Platforms/On (32x10).png");
    Arrow::check_image_existence(s_on);
    rect.w = 32;
    rect.h = 10;
}
Falling_platform::~Falling_platform()
{
    SDL_FreeSurface(s_off);
    SDL_FreeSurface(s_on);
    image = s_off = s_on = NULL;
}

void Falling_platform::draw(SDL_Surface* screen)
{
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = { Sint16(x-viewport->x), Sint16(y-viewport->y) };
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Falling_platform::update()
{
    image = off?s_off:s_on;
    if (timer.get_elapsed_ms() >= 75)
    {
        cur_image = (cur_image+1)%4;
        timer.restart();
    }
    if (y_vel)
    {
        if (y<=y0-5)
            y_vel = 0.5;
        else if (y>=y0+5)
            y_vel = -0.5;
    }
    y += y_vel;
}
