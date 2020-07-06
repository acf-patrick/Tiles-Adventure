#include <algorithm>
#include <cstdlib>
#include "sprite.h"
#include "func_tool.h"

Sprite::Sprite(SDL_Surface* surface):
    check_horizontal(false), check_up(false), check_down(false)
{
    if (surface)
    {
        image = flip(surface, false, false);
        rect.w = surface->w;
        rect.h = surface->h;
    }
    else
    {
        image = NULL;
        rect.w = 0;
        rect.h = 0;
    }
    rect.x = x = 0;
    rect.y = y = 0;
    type.push_back("basic_sprite");
}

Sprite::Sprite(SDL_Rect r)
{
    x = r.x;
    y = r.y;
    rect.x = (Sint16)x;
    rect.y = (Sint16)y;
    rect.w = r.w;
    rect.h = r.h;
    image = NULL;
    type.push_back("basic sprite");
}

Sprite::~Sprite()
{
    SDL_FreeSurface(image);
    image = NULL;
}

int Sprite::get_x() { return x; }
int Sprite::get_y() { return y; }
int Sprite::get_centerx() { return x+rect.w/2.; }
int Sprite::get_centery() { return y+rect.h/2.; }
int Sprite::get_right() { return x+rect.w; }
int Sprite::get_bottom() { return y+rect.h; }
float *Sprite::get_impulse()
{
    float *ret = (float*)malloc(2*sizeof (float));
    ret[0] = ret[1] = 0;
    return ret;
}
SDL_Surface* Sprite::get_surface() { return image; }
SDL_Rect Sprite::get_rect()
{
    SDL_Rect r = {Sint16(x), Sint16(y), rect.w, rect.h};
    return r;
}
std::string Sprite::get_type() { return type[type.size()-1]; }
std::string Sprite::get_ancestor()
{
    if (type.size() < 2)
        return get_type();
    return type[type.size()-2];
}
bool Sprite::is(const std::string& specified_type) { return (type.end()!=std::find(type.begin(), type.end(), specified_type)); }

void Sprite::set_x(Sint16 _x) { x = _x; }
void Sprite::set_y(Sint16 _y) { y = _y; }
void Sprite::set_position(int _x, int _y) { x = _x, y = _y; }
void Sprite::move(int x_offset, int y_offset)
{
    x += x_offset;
    y += y_offset;
}
void Sprite::set_right(Sint16 right) { x = right - rect.w; }
void Sprite::set_bottom(Sint16 bottom) { y = bottom - rect.h; }
void Sprite::set_center(Sint16 _x, Sint16 _y)
{
    set_x(_x - rect.w*.5);
    set_y(_y - rect.h*.5);
}

void Sprite::bump(const std::string& flag)
{
    // override
}

bool Sprite::is_out()
{
    // override
    return false;
}

bool Sprite::alive()
{
    for (int i=0; i<(int)groups.size(); ++i)
        if (groups[i]->has(this))
            return true;
    return false;
}

void Sprite::add(Group* group)
{
    if (!group)
        return;
    if (groups.end() == std::find(groups.begin(), groups.end(), group))
    {
        groups.push_back(group);
        group->add(this);
    }
}

void Sprite::kill()
{
    for (int i=0; i<(int)groups.size(); ++i)
        groups[i]->remove(this);
    groups.clear();
}

bool Sprite::collide_with(Sprite* sprite)
{
    if (sprite == NULL or sprite == this)
        return false;

    rect.x = (Sint16)x;
    sprite->rect.x = (Sint16)sprite->x;
    rect.y = (Sint16)y;
    sprite->rect.y = (Sint16)sprite->y;
    return rects_collide(sprite->rect, rect);
}

bool Sprite::collide_with(SDL_Rect r)
{
    rect.x = (Sint16)x;
    rect.y = (Sint16)y;
    return rects_collide(r, rect);
}

bool Sprite::collide_with(int p_x, int p_y)
{
    rect.x = (Sint16)x;
    rect.y = (Sint16)y;
    return rect_n_point_collide(rect, p_x, p_y);
}

void Sprite::update()
{
    //override
}

void Sprite::draw(SDL_Surface* screen)
{
    rect.x = Sint16(x);
    rect.y = Sint16(y);
    SDL_BlitSurface(image, NULL, screen, &rect);
}

void Sprite::remember(Group* group) { groups.push_back(group); }
