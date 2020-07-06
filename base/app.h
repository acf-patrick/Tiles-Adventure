#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include <SDL.h>

/// Template
class App
{
public:
    static int width, height;
    static SDL_Event event;

    App(std::string app_title = "App", int w = 480, int h = 360);
    virtual ~App();
    void run();
    void pause();

protected:
    virtual void manage_events();
    virtual void update();
    virtual void draw();

    bool paused;
    bool running;
    SDL_Surface* screen;
    bool keys[SDLK_LAST];

    virtual void update_events();
};

#endif
