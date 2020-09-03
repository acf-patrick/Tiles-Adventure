#include <SDL/SDL_image.h>
#include <ctime>
#include "base/func_tool.h"
#include "base/app.h"
#include "fruits.h"

Fruit::Fruit(int _x, int _y, Map* m, const std::string& name, bool hidden):
    With_mass(m, _x, _y),
    collected(false), cur_image(0),
    static_item(!hidden)
{
    if (!m_map)
    {
        viewport = new SDL_Rect;
        viewport->x = 0;
        viewport->y = 0;
        int ww, wh;
        App::instance->window_size(&ww, &wh);
        viewport->w = ww;
        viewport->h = wh;
    }
    else
        viewport = m_map->get_viewport();
    if (hidden)
    {
        y_vel = -6;
        x_vel = ((rand()%2)?1:-1)*8;
    }
    type.push_back("Fruits");
    type.push_back(name);
    rect.w = rect.h = 32;
    std::string img_name("images/Items/Fruits/");
    img_name += name + ".png";
    image = IMG_Load(img_name.c_str());
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
}

void Fruit::draw(SDL_Surface* screen)
{
    SDL_Rect pos = { Sint16(x - viewport->x), Sint16(y - viewport->y) };
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Fruit::update()
{
    if (timer.get_elapsed_ms() >= 50)
    {
        cur_image++;
        timer.restart();
    }
    int n(image->w/rect.w);
    if (collected and cur_image>=n)
        kill();

    if (!static_item)
    {
        apply_gravity();
        move(x_vel, y_vel);
        y += 2;
        if (m_map->collision_with(this))
            static_item = true;
        y -= 2;
    }

    cur_image %= n;
}

void Fruit::bump(const std::string& flag)
{
    if (!collected)
    {
        collected = true;
        cur_image = 0;
        SDL_FreeSurface(image);
        image = IMG_Load("images/Items/Fruits/Collected.png");
        if (!image)
        {
            std::cerr << IMG_GetError();
            exit(EXIT_FAILURE);
        }
    }
}
