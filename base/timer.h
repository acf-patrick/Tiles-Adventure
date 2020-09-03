#ifndef TIMER_H
#define TIMER_H

<<<<<<< HEAD
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
=======
#include <SDL.h>
#include <SDL_framerate.h>
>>>>>>> 495eb918d6ae1f887b19d2afaa37aeac42acb24c
#include "text.h"

class Timer
{
public:
    Timer();
    int get_elapsed_ms();
    float get_elapsed_s();
    void restart();

protected:
    int start_time;
};

class Chrono: public Timer
{
public:
    /// @param Temps en secondes
    Chrono(int);
    void restart(int r_time = -1);
    bool out();

private:
    int remaining;
};

class Fps: public Text
{
public:
    static const int DEFAULT;
    static const int UPPER_LIMIT;
    static const int LOWER_LIMIT;
    Fps(Uint32 _rate = DEFAULT, const std::string& font_name = "arial");
    void update();

private:
    Uint32 rate;
    Uint32 base_ticks;
    int frame_count;
    long int frames;
    float rate_ticks;
};

#endif
