#include <SDL.h>
#include <SDL_ttf.h>
#include "app.h"
#include "func_tool.h"
#include "widget.h"

App* App::instance(NULL);

SDL_Event* App::get_event() { return &event; }
void App::window_size(int* w, int* h)
{
    if (w) *w = screen->w;
    if (h) *h = screen->h;
}

App::App(std::string app_title, int w, int h):
    paused(false), running(true)
{
    instance = this;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_putenv("SDL_VIDEO_CENTERED=1");

    screen = SDL_SetVideoMode( w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (!screen)
    {
        std::cerr << SDL_GetError();
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption(app_title.c_str(), NULL);

    for (int i=0; i<SDLK_LAST; ++i)
        keys[i] = false;
}

App::~App()
{
    TTF_Quit();
    SDL_Quit();
}

void App::run()
{
    while (running)
    {
        while (SDL_PollEvent(&event))
            update_events();
        manage_events();
        update();
        draw();
        SDL_Flip(screen);
    }
}

void App::manage_events()
{
    //override
}

void App::update()
{
    //override
}

void App::draw()
{
    //override
}

void App::update_events()
{
    switch(event.type)
    {
    case SDL_QUIT:
        end();
        break;
    case SDL_KEYDOWN:
        keys[event.key.keysym.sym] = true;
        if (keys[SDLK_ESCAPE])
            end();
        break;
    case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        break;
    default: ;
    }
}

void App::pause() { paused = !paused; }
void App::end()
{
    if (confirm("Quitter?"))
        running = false;
}
