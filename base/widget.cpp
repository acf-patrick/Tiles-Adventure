#include <SDL/SDL_gfxPrimitives.h>
#include "func_tool.h"
#include "widget.h"

bool confirm(const std::string& message)
{
    SDL_Surface* screen(SDL_GetVideoSurface());
    Confirm widget(message);
    while (widget.result < 0)
    {
        SDL_PollEvent(App::instance->get_event());
        widget.update();
        widget.draw(screen);
        SDL_Flip(screen);
    }
    return widget.result;
}

Button::Button(const std::string& text)
{
    state = IDLE;
    innerText = new Text(text, 255, 255, 255, "fonts/Supercell-Magic_5", NULL, 15, 0, 0);
    SDL_Rect _(innerText->get_rect());
    rect.w = _.w+20;
    rect.h = _.h+20;
    define_bg();
}
void Button::action()
{
    // override
}

__button__::~__button__()
{
    for (int i=0; i<3; ++i)
    {
        SDL_FreeSurface(background[i]);
        background[i] = NULL;
    }
    delete innerText;
    innerText = NULL;
}

void __button__::define_bg(SDL_Surface* hover, SDL_Surface* clicked, SDL_Surface* idle)
{
    if (idle)
        background[IDLE] = idle;
    else
    {
        background[IDLE] = createSurface(rect.w, rect.h);
        roundedBoxColor(background[IDLE], 0, 0, rect.w, rect.h, 10, 0x262626ff);
    }
    if (hover)
        background[HOVER] = hover;
    else
    {
        background[HOVER] = createSurface(rect.w, rect.h);
        roundedBoxColor(background[HOVER], 0, 0, rect.w, rect.h, 10, 0x464646ff);
    }
    if (clicked)
        background[CLICKED] = clicked;
    else
    {
        background[CLICKED] = createSurface(rect.w, rect.h);
        roundedBoxColor(background[CLICKED], 0, 0, rect.w, rect.h, 10, 0x7c7c7cff);
    }
}

void __button__::draw(SDL_Surface* screen)
{
    image = background[state];
    GameObject::draw(screen);
    image = NULL;
    innerText->draw(screen);
}

void __button__::update()
{
    int bx, by;
    SDL_GetMouseState(&bx, &by);
    SDL_Event* event(App::instance->get_event());
    if (collide_with(bx, by))
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (event->type == SDL_MOUSEBUTTONDOWN)
                state = CLICKED;
            else
            {
                if (event->type == SDL_MOUSEBUTTONUP)
                    if (state == CLICKED)
                        action();
                state = HOVER;
            }
        }
        else state = HOVER;
    }
    else
        state = IDLE;
    innerText->set_center(get_centerx(), get_centery());
}

void __button__::set_position(int _x, int _y)
{
    GameObject::set_position(_x, _y);
    innerText->set_center(get_centerx(), get_centery());
}

void __button__::set_size(float e)
{
    innerText->setSize(e);
    SDL_Rect txtRect(innerText->get_rect());
    rect.w = txtRect.w+20;
    rect.h = txtRect.h+20;
    innerText->set_center(get_centerx(), get_centery());
    define_bg();
}

void __button__::set_text(const std::string& t)
{
    SDL_Color white = {255, 255, 255};
    innerText->set(white, t);
    innerText->set_center(get_centerx(), get_centery());
}

Confirm::Confirm(const std::string& text): result(-1)
{
    /* création des composants */
    message = new Text(text, 255, 255, 255, "fonts/Supercell-magic_5", NULL, 15, 0, 0);
    ok = new Ok(this);
    cancel = new Cancel(this);

    /* définition des tailles et de la position de l'objet */
    SDL_Rect msg_rect(message->get_rect()), ok_rect(ok->get_rect());
    rect.w = 2*(ok_rect.w+20);
    int diff(0.5*(msg_rect.w-rect.w));
    if (diff >= 0) rect.w += 2*(10+diff);
    rect.h = msg_rect.h+ok_rect.h+40;
    int ww, wh;
    App::instance->window_size(&ww, &wh);
    x = 0.5*(ww-rect.w);
    y = 0.5*(wh-rect.h);

    /* mise en place des composants */
    ok->set_x(10);
    ok->set_bottom(rect.h-10);
    cancel->set_right(rect.w-10);
    cancel->set_bottom(rect.h-10);
    message->set_center(0.5*rect.w, message->get_centery());
    message->set_y(10);

    /* fond */
    image = createSurface(rect.w, rect.h);
    roundedBoxColor(image, 0, 0, rect.w, rect.h, 10, 0xa8a8aa);

    /* convertion en coordonnées globales */
    ok->move(x, y);
    cancel->move(x, y);
    message->move(x, y);
}

Confirm::~Confirm()
{
    delete ok; ok = NULL;
    delete cancel; cancel = NULL;
    delete message; message = NULL;
}
void Confirm::update()
{
    ok->update();
    cancel->update();
    message->update();
}
void Confirm::draw(SDL_Surface* screen)
{
    GameObject::draw(screen);
    ok->draw(screen);
    cancel->draw(screen);
    message->draw(screen);
}
