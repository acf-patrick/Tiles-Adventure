#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "sprite.h"

class Text: public Sprite
{
public:
    Text(const std::string& content, int r, int g, int b, std::string font_name,
         SDL_Rect* _viewport, int char_size, int _x, int _y);
    virtual ~Text();

    void draw(SDL_Surface*);
    void set(SDL_Color, const std::string& content = "");

protected:

    std::string text;
    SDL_Color color;
    TTF_Font* font;
    SDL_Rect* viewport;
};

#endif
