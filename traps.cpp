#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include "base/app.h"
#include "base/map.h"
#include "base/func_tool.h"
#include "base/with_mass.h"
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

SDL_Rect* Arrow::create_static_viewport()
{
    SDL_Rect *ret = new SDL_Rect;
    if (!ret)
        return NULL;
    ret->x = ret->y = 0;
    int ww, wh;
    App::instance->window_size(&ww, &wh);
    ret->w = ww;
    ret->h = wh;
    return ret;
}

Falling_platform::Falling_platform(SDL_Rect* v, int _x, int _y):
    Basic_fan(v, _x, _y)
{
    sens = Bubbles::BAS;
    type.push_back("Falling Platform");

    y0 = y;
    y_vel = 0.5;
    gravity = 0;
    rect.w = 32;
    rect.h = 10;
    s_off = IMG_Load("images/Traps/Falling Platforms/Off.png");
    check_image_existence(s_off);
    s_on = IMG_Load("images/Traps/Falling Platforms/On (32x10).png");
    check_image_existence(s_on);
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

bool Falling_platform::collide_with(GameObject* sprite)
{
    if (!sprite or extinction)
        return false;
    return GameObject::collide_with(sprite);
}

Group* Basic_fan::bubbles = new Group;

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
            if (cur_image%2)
                bubbles->add(new Bubbles(viewport, get_rect(), sens, min_tar, max_tar));
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

void Basic_fan::draw_bubbles(SDL_Surface* screen) { bubbles->draw(screen); }
void Basic_fan::update_bubbles(Map* m)
{
    std::vector<GameObject*> bulles(bubbles->sprites());
    for (int i=0; i<(int)bulles.size(); ++i)
    {
        if (m->collision_with(bulles[i]))
            bulles[i]->kill();
        else
            bulles[i]->update();
    }
}
void Basic_fan::destroy_bubbles()
{
    delete bubbles;
    bubbles = NULL;
}

Fan::Fan(SDL_Rect* v, int _x, int _y, GameObject* cible, int angle):
    Basic_fan(v, _x, _y),
    switch_timer(randint(5, 10)),
    target(cible)
{
    type.push_back("Fan");
    sens = angle/90;
    SDL_Surface* tmp(IMG_Load("images/Traps/Fan/Off.png"));
    check_image_existence(tmp);
    y += tmp->h;
    int cx(x+.5*tmp->w) ,cy(y-.5*tmp->h);
    apply_rotation(x, y, angle, &cx, &cy);

    s_off = rotateSurface90Degrees(tmp, sens);
    s_on = rotateSurface90Degrees(IMG_Load("images/Traps/Fan/On (24x8).png"), sens);
    check_image_existence(s_on);

    rect.w = s_off->w;
    rect.h = s_off->h;
    x = cx - .5*rect.w;
    y = cy - .5*rect.h;
    min_tar = 200;
    max_tar = 300;
    field.x = x;
    field.y = y;
    if (sens==Bubbles::HAUT or sens==Bubbles::BAS)
    {
        field.w = rect.w;
        field.h = 250;
    }
    else if (sens==Bubbles::GAUCHE or sens==Bubbles::DROITE)
    {
        field.w = 250;
        field.h = rect.h;
    }
    if (sens == Bubbles::HAUT)
        field.y -= field.h;
    else if (sens == Bubbles::BAS)
        field.y += rect.h;
    else if (sens == Bubbles::GAUCHE)
        field.x -= field.w;
    else if (sens == Bubbles::DROITE)
        field.x += rect.w;
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

void Fan::draw(SDL_Surface* screen)
{
    SDL_Rect pos = { Sint16(x-viewport->x), Sint16(y-viewport->y) };
    if (off)
        rect.x = rect.y = 0;
    else
        switch (sens)
        {
        case Bubbles::HAUT:
            rect.x = cur_image*rect.w;
            rect.y = 0;
            break;
        case Bubbles::BAS:
            rect.x = (3-cur_image)*rect.w;
            rect.y = 0;
            break;
        case Bubbles::GAUCHE:
            rect.x = 0;
            rect.y = (3-cur_image)*rect.h;
            break;
        case Bubbles::DROITE:
            rect.x = 0;
            rect.y = cur_image*rect.h;
            break;
        default: ;
        }
    SDL_BlitSurface(image, &rect, screen, &pos);
    pos.x = field.x-viewport->x;
    pos.y = field.y-viewport->y;
    if (!m_static and With_mass::showBoundingBox)
        rectangleRGBA(screen, pos.x, pos.y, pos.x+field.w, pos.y+field.h, 0, 0, 222, 255);
}

Spike::Spike(SDL_Rect* v, int _x, int _y, int angle):
    viewport(v)
{
    type.push_back("Spike");
    SDL_Surface* tmp(IMG_Load("images/Traps/Spikes/Idle.png"));
    check_image_existence(tmp);
    x = _x;
    y = _y + tmp->h;
    _x += .5*tmp->w;
    _y += .5*tmp->h;
    apply_rotation(x, y, angle, &_x, &_y);

    image = rotozoomSurface(tmp, angle, 1, true);
    rect.w = image->w;
    rect.h = image->h;
    x = _x - .5*rect.w;
    y = _y - .5*rect.h;
}

void Spike::draw(SDL_Surface* screen)
{
    SDL_Rect pos = { (Sint16)(x-viewport->x), (Sint16)(y-viewport->y) };
    SDL_BlitSurface(image, NULL, screen, &pos);
}

SDL_Surface* Spike::get_surface()
{
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32,
                        image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
    SDL_FillRect(ret, NULL, SDL_MapRGB(ret->format, 100, 100, 100));
    SDL_BlitSurface(image, NULL, ret, 0);
    return ret;
}

bool Spike::collide_with(GameObject* sprite)
{
    if (!sprite)
        return false;
    if (!sprite->is("Player"))
        return false;
    SDL_Surface* s1 = get_surface(), *s2 = sprite->get_surface();
    bool ret( images_collide(x-viewport->x, y-viewport->y, s1,
                sprite->get_x()-viewport->x, sprite->get_y()-viewport->y, s2) );
    SDL_FreeSurface(s1);
    SDL_FreeSurface(s2);
    return ret;
}
