#include <SDL/SDL_image.h>
#include "base/func_tool.h"
#include "show.h"

int Show::instance(0);

Show::Show(int x0, int y0, bool appear):
    desappear(!appear)
{
    if (appear)
        instance++;
    rect.w = rect.h = 96;
    x = x0 - .5*rect.w;
    y = y0 - .5*rect.h;
    cur_image = 0;
    std::string name("images/Main Characters/");
    name += appear?"Appearing":"Desappearing";
    name += " (96x96).png";
    image = IMG_Load(name.c_str());
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
    bnw_screenshot = NULL;
}

Show::~Show()
{
    instance--;
    SDL_FreeSurface(bnw_screenshot);
}

bool Show::is_out() { return cur_image >= 7; }

void Show::update()
{
    if (timer.get_elapsed_ms() >= 100)
    {
        cur_image++;
        timer.restart();
    }
}

void Show::draw(SDL_Surface* screen)
{
    if (!bnw_screenshot and !desappear)
        bnw_screenshot = black_n_white(copy_surface(screen));

    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = {Sint16(x), Sint16(y)};
    SDL_BlitSurface(bnw_screenshot, NULL, screen, 0);
    SDL_BlitSurface(image, &rect, screen, &pos);
}
