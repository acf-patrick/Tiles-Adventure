#include <SDL_image.h>
#include "base/app.h"
#include "fruits.h"

Fruit::Fruit(int _x, int _y, SDL_Rect* v, const std::string& name):
    viewport(v),
    collected(false), cur_image(0)
{
    if (!v)
    {
        viewport = new SDL_Rect;
        viewport->x = 0;
        viewport->y = 0;
        viewport->w = App::width;
        viewport->h = App::height;
    }
    type.push_back("Fruits");
    type.push_back(name);
    x = _x;
    y = _y;
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
