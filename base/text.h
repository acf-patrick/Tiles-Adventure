#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "object.h"

class Text: public GameObject
{
public:
    Text(const std::string& content, int r, int g, int b, const std::string& font_name,
         SDL_Rect* _viewport, int char_size, int _x, int _y);
    virtual ~Text();

    void draw(SDL_Surface*);
    void set(SDL_Color, const std::string& content = "");
    void setFont(const std::string& font_name, int textSize = 0);
    void setSize(float);

protected:

    SDL_Rect* viewport;
    std::string text, fontName;
    SDL_Color color;
    TTF_Font* font;
};

#endif
