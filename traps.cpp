#include <SDL_image.h>
#include "base/app.h"
#include "base/func_tool.h"
#include "traps.h"
#include "level.h"

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
    Basic_fan(v, _x, _y)
{
    ascendant = false;
    type.push_back("Falling Platform");

    y0 = y;
    y_vel = 0.5;
    gravity = 0;
    rect.w = 32;
    rect.h = 10;
    s_off = IMG_Load("images/Traps/Falling Platforms/Off.png");
    Arrow::check_image_existence(s_off);
    s_on = IMG_Load("images/Traps/Falling Platforms/On (32x10).png");
    Arrow::check_image_existence(s_on);
}

void Falling_platform::update()
{
    if (off)
    {
        if (y>=viewport->y+viewport->h)
            return kill();
        image = s_off;
    }
    else
    {
        Basic_fan::update();
        if (extinction)
            if (animation_delay>=100)
                turn_off();
        if (y<=y0-5 or y>=y0+5)
            y_vel *= -1;
    }
    if (!m_static)
    {
        y_vel += gravity;
        y += y_vel;
    }
}

void Falling_platform::turn_off()
{
    cur_image = 0;
    y_vel = 1;
    gravity = 0.5;
    off = true;
}

void Falling_platform::bump(const std::string& flag)
{
    extinction = true;
}

bool Falling_platform::collide_with(Sprite* sprite)
{
    if (!sprite or extinction)
        return false;
    return Sprite::collide_with(sprite);
}

Group *Basic_fan::bubbles(NULL);

Basic_fan::Basic_fan(SDL_Rect* v, int _x, int _y):
    viewport(v), off(false),
    cur_image(0), extinction(false),
    demarrage(true)
{
    if (!viewport)
    {
        off = true;
        viewport = Arrow::create_static_viewport();
    }
    m_static = !v;
    type.push_back("Basic Fan");
    x = _x;
    y = _y;
    min_tar = 100;
    max_tar = 130;
    animation_delay = 100;
}
Basic_fan::~Basic_fan()
{
    SDL_FreeSurface(s_off);
    SDL_FreeSurface(s_on);
    image = s_off = s_on = NULL;
}

void Basic_fan::update()
{
    if (!off)
    {
        image = s_on;
        if (demarrage)
            if (animation_delay>40)
                animation_delay--;
            else
                demarrage = false;
        else if (extinction)
        {
            if (animation_delay>=200)
            {
                off = true;
                extinction = false;
            }
            animation_delay += 2;
        }

        if (timer.get_elapsed_ms() >= animation_delay)
        {
            cur_image = (cur_image+1)%4;
            if (cur_image%2 and bubbles)
                bubbles->add(new Bubbles(viewport, get_rect(), ascendant, min_tar, max_tar));
            timer.restart();
        }
    }
    else
    {
        cur_image = 0;
        image = s_off;
    }
}

void Basic_fan::draw(SDL_Surface* screen)
{
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = { Sint16(x-viewport->x), Sint16(y-viewport->y) };
    SDL_BlitSurface(image, &rect, screen, &pos);
}

Fan::Fan(SDL_Rect* v, int _x, int _y, int sens):
    Basic_fan(v, _x, _y),
    switch_timer(randint(5, 10))
{
    type.push_back("Fan");
    ascendant = true;
    rect.w = 24;
    rect.h = 8;
    s_off = IMG_Load("images/Traps/Fan/Off.png");
    Arrow::check_image_existence(s_off);
    s_on = IMG_Load("images/Traps/Fan/On (24x8).png");
    Arrow::check_image_existence(s_on);
    min_tar = 200;
    max_tar = 300;
}

void Fan::update()
{
    if (!m_static)
        if (switch_timer.out())
        {
            int delay;
            if (off)
            {
                delay = randint(10, 20);
                demarrage = true;
                off = false;
            }
            else
            {
                delay = randint(5, 10);
                extinction = true;
            }
            switch_timer.restart(delay);
        }
    Basic_fan::update();
}
