#ifndef APP_H
#define APP_H

<<<<<<< HEAD
#include <SDL/SDL.h>
#include <iostream>
#include <vector>
=======
#include <iostream>
#include <vector>
#include <SDL.h>
>>>>>>> 495eb918d6ae1f887b19d2afaa37aeac42acb24c

/// Template
class App
{
public:
    static App* instance;

    App(std::string app_title = "App", int w = 480, int h = 360);
    virtual ~App();
    void run();
    void pause();
    void end();
    SDL_Event* get_event();
    void window_size(int*, int*);

protected:
    virtual void manage_events();
    virtual void update();
    virtual void draw();

    bool paused;
    bool running;
    SDL_Surface* screen;
    SDL_Event event;
    bool keys[SDLK_LAST];

    virtual void update_events();
};

#endif
