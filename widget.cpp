#include "base/app.h"
#include "widget.h"

Button::Button(const std::string& text): state(IDLE)
{
    innerText = new Text(text, 255, 255, 255, "fonts/Supercell-Magic_5", NULL, 15, 0, 0);
    SDL_Rect _(innerText->get_rect());
    rect.w = _.w+20;
    rect.h = _.h+20;
    define_bg();
    set_position(App::width-rect.w, App::height-rect.h);
}
Button::~Button()
{
    for (int i=0; i<3; ++i)
    {
        SDL_FreeSurface(background[i]);
        background[i] = NULL;
    }
    delete innerText;
    innerText = NULL;
}

void Button::define_bg()
{
    for (int i=0; i<3; ++i)
        background[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32, 0, 0, 0, 0);
    SDL_FillRect(background[IDLE], NULL, 0x262626);
    SDL_FillRect(background[HOVER], NULL, 0x464646);
    SDL_FillRect(background[CLICKED], NULL, 0x7c7c7c);
}

void Button::draw(SDL_Surface* screen)
{
    image = background[state];
    Sprite::draw(screen);
    image = NULL;
    innerText->draw(screen);
}

void Button::update()
{
    int bx(App::event.motion.x), by(App::event.motion.y);
    if (collide_with(bx, by))
    {
        if (App::event.button.button == SDL_BUTTON_LEFT)
        {
            if (App::event.type == SDL_MOUSEBUTTONDOWN)
                state = CLICKED;
            else
            {
                state = HOVER;
                if (App::event.type == SDL_MOUSEBUTTONUP)
                    exit(0);
            }
        }
        else state = HOVER;
    }
    else
        state = IDLE;
}

void Button::set_position(int _x, int _y)
{
    Sprite::set_position(_x, _y);
    innerText->set_center(get_centerx(), get_centery());
}
