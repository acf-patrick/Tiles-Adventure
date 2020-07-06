#ifndef WIDGET_H
#define WIDGET_H

#include "base/sprite.h"
#include "base/text.h"
#include <SDL.h>

class Button: public Sprite
{
public:
    Button(const std::string&);
    ~Button();
    void set_position(int, int);
    void draw(SDL_Surface*);
    void update();

private:
    void define_bg();
    enum { HOVER, CLICKED, IDLE };
    int state;
    SDL_Surface* background[3];
    Text* innerText;
};

#endif // WIDGET_H
