#ifndef APP_H
#define APP_H

#include <iostream>
#include <vector>
#include <SDL.h>

/// Classe de base
class App
{
public:
    static int width, height;

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
    SDL_Event event;
    bool keys[SDLK_LAST];

    // true: pressed
    // false: released
    bool button[6];

    virtual void update_events();
};

#endif
