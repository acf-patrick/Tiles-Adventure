#include <SDL_gfxPrimitives.h>
#include "with_mass.h"
#include "func_tool.h"

bool With_mass::showBoundingBox(false);

With_mass::With_mass(Map* m, int x0, int y0):
    m_map(m), GRAVITY(0.8)
{
    gravity = GRAVITY;
    x = x0;
    y = y0;
    x_vel = 0;
    y_vel = 0;
    rect.w = rect.h = 0;
    direction = RIGHT;
    type.push_back("basic mass");
}

void With_mass::draw(SDL_Surface* screen)
{
    show_boundingBox(screen);
    int xs((!m_map)?0:m_map->get_xshift()), ys((!m_map)?0:m_map->get_yshift());
    SDL_Rect pos = {(Sint16)(x - xs), (Sint16)(y - ys)};
    SDL_BlitSurface(image, NULL, screen, &pos);
}

void With_mass::show_boundingBox(SDL_Surface* screen)
{
    Uint32 color;
    if (is("Enemy"))
        color = 0xcd4f4f;
    else if (is("Player"))
        color = 0xffffff;
    else if (is("Bullet"))
        color = 0x0;
    else
        color = 0x5a5fc0;
    if (!showBoundingBox)
        return;
    int xs((!m_map)?0:m_map->get_xshift()), ys((!m_map)?0:m_map->get_yshift());
    int x1 = x-xs,
        y1 = y-ys;
    int x2 = x1 + rect.w,
        y2 = y1 + rect.h;
    rectangleColor(screen, x1, y1, x2, y2, color);
}

void With_mass::set_map(Map* m) { m_map = m; }

void With_mass::move(int x_offset, int y_offset)
{
    if (!m_map)
        return GameObject::move(x_offset, y_offset);
    if (x_offset >= m_map->get_tile_w() or y_offset >= m_map->get_tile_h())
    {
        move(x_offset/2, y_offset/2);
        move(x_offset-x_offset/2, y_offset-y_offset/2);
        return;
    }
    if (try_move(x_offset, y_offset))
        return;
    affine(x_offset, y_offset);
}

bool With_mass::try_move(int x_offset, int y_offset)
{
    if (!m_map)
        return true;
    x += x_offset;
    y += y_offset;
    if (!m_map->collision_with(this))
        return true;
    x -= x_offset;
    y -= y_offset;
    return false;
}

void With_mass::affine(int x_offset, int y_offset)
{
    for (int i=0; i<abs(x_offset); ++i)
        if (!try_move(sgn(x_offset), 0))
            break;
    for (int i=0; i<abs(y_offset); ++i)
        if (!try_move(0, sgn(y_offset)))
            break;
}

void With_mass::apply_gravity()
{
    if (y_vel == 0)
        y_vel = 1;
    else
        y_vel += gravity;
}
