#include <sstream>
#include "timer.h"
#include "app.h"

Uint32 getTicks()
{
    Uint32 ret(SDL_GetTicks());
    if (!ret)
        return 1;
    return ret;
}

Timer::Timer() { restart(); }

int Timer::get_elapsed_ms(){ return (SDL_GetTicks() - start_time + 1); }
float Timer::get_elapsed_s(){ return get_elapsed_ms()/1000.; }

void Timer::restart() { start_time = SDL_GetTicks(); }

Chrono::Chrono(int remaining_time):
    remaining(remaining_time)
{}
void Chrono::restart(int r_time)
{
    if (r_time>0)
        remaining = r_time;
    Timer::restart();
}
bool Chrono::out() { return (get_elapsed_s()>=remaining); }

const int Fps::LOWER_LIMIT(1);
const int Fps::DEFAULT(30);
const int Fps::UPPER_LIMIT(200);

Fps::Fps(Uint32 _rate, const std::string& font_name):
    Text("FPS : ", 255, 0, 0, font_name, NULL, 10, -200, -50),
    rate(_rate)
{
    int _x, _y;
    App::instance->window_size(&_x, &_y);
    x += _x; y += _y;
    rate_ticks = 1000./rate;
    frame_count = frames = 0;
    base_ticks = SDL_GetTicks()+1;
}

void Fps::update()
{
    frame_count++;
    frames++;
    Uint32 target_ticks(base_ticks + frame_count*rate_ticks);
    Uint32 cur_ticks(SDL_GetTicks()+1);

    if (cur_ticks <= target_ticks)
        SDL_Delay(target_ticks - cur_ticks);
    else
    {
        frame_count = 0;
        base_ticks = cur_ticks;
    }
    std::stringstream new_text;
    new_text << "FPS : " << 1000*frames/cur_ticks;
    set(color, new_text.str());
}
