#include <vector>
#include "app.h"
#include "text.h"
#include "func_tool.h"

Text::Text(const std::string& content, int r, int g, int b, std::string font_name, SDL_Rect* _viewport, int char_size, int _x, int _y)
{
    if (_viewport)
        viewport = _viewport;
    else
    {
        viewport = new SDL_Rect;
        viewport->x = 0;
        viewport->y = 0;
        viewport->w = App::width;
        viewport->h = App::height;
    }
    if (!TTF_WasInit())
    {
        if (TTF_Init())
        {
            std::cerr << TTF_GetError();
            exit(EXIT_FAILURE);
        }
    }
    font_name += ".ttf";
    font = TTF_OpenFont(font_name.c_str(), char_size);
    if (!font)
    {
        std::cerr << TTF_GetError();
        exit(EXIT_FAILURE);
    }
    SDL_Color _color = {Uint8(r), Uint8(g), Uint8(b)};
    set(_color, content);
    x = _x;
    y = _y;
    rect.w = image->w;
    rect.h = image->h;
    type.push_back("text");
}

Text::~Text()
{
    SDL_FreeSurface(image);
    image = NULL;
    TTF_CloseFont(font);
    font = NULL;
}

void Text::set(SDL_Color _color, const std::string& content)
{
    if (content.size())
        text = content;
    color.r = _color.r;
    color.g = _color.g;
    color.b = _color.b;
    std::vector<std::string> names(split(content, "\n"));
    if (names[0] != content)
    {
        std::vector<SDL_Surface*> images;
        Uint16 max_w(0), height(0);
        for (int i=0; i<(int)names.size(); ++i)
        {
            images.push_back(TTF_RenderText_Solid(font, names[i].c_str(), color));
            if (images[i]->w > max_w)
                max_w = images[i]->w;
            height += images[i]->h;
        }
        SDL_FreeSurface(image);
        image = SDL_CreateRGBSurface(SDL_HWSURFACE, max_w, height, 32, 0, 0, 0, 0);
        SDL_Rect pos = {0};
        for (int i=0; i<(int)images.size(); ++i)
        {
            SDL_BlitSurface(images[i], NULL, image, &pos);
            pos.y += images[i]->h;
        }
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, 0x0);
    }
    else
    {
        SDL_FreeSurface(image);
        image = TTF_RenderText_Solid(font, content.c_str(), color);
    }
}

void Text::draw(SDL_Surface* screen)
{
    rect.x = Sint16(x-viewport->x);
    rect.y = Sint16(y-viewport->y);
    SDL_BlitSurface(image, NULL, screen, &rect);
}
