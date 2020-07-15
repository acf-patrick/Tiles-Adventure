#include "base/func_tool.h"
#include "checkpoints.h"
#include "base/app.h"
#include <SDL_image.h>
#include <sstream>

Checkpoints::Checkpoints(const std::string& name, int _x, int _y, SDL_Rect* v):
    viewport(v), cur_image(0), state("Idle")
{
    if (!v)
    {
        viewport = new SDL_Rect;
        viewport->x = 0;
        viewport->y = 0;
        int ww, wh;
        App::instance->window_size(&ww, &wh);
        viewport->w = ww;
        viewport->h = wh;
    }
    type.push_back("Checkpoints");
    type.push_back(name);
    x = _x;
    y = _y;
    rect.w = rect.h = 64;
}

Checkpoints::~Checkpoints()
{
    for (std::map<std::string, SDL_Surface*>::iterator it=images.begin(); it!=images.end(); ++it)
    {
        SDL_FreeSurface(it->second);
        images[it->first] = NULL;
    }
    image = NULL;
}

void Checkpoints::draw(SDL_Surface* screen)
{
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos;
    pos.x = Sint16(x-viewport->x);
    pos.y = Sint16(y-viewport->y);
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Checkpoints::update()
{
    image = images[state];
    if (timer.get_elapsed_ms() >= 75)
    {
        cur_image = (cur_image+1)%(image->w/rect.w);
        timer.restart();
    }
}

Checkpoint::Checkpoint(int _x, int _y, SDL_Rect* v):
    Checkpoints("Checkpoint", _x, _y, v)
{
    state = "Empty";
    images["Idle"] = IMG_Load("images/Items/Checkpoints/Checkpoint/Checkpoint (Flag Idle) (64x64).png");
    images["Out"] = IMG_Load("images/Items/Checkpoints/Checkpoint/Checkpoint (Flag Out) (64x64).png");
    images["Empty"] = IMG_Load("images/Items/Checkpoints/Checkpoint/Checkpoint (No Flag).png");
}

void Checkpoint::update()
{
    image = images[state];
    if (timer.get_elapsed_ms() >= 50)
    {
        cur_image++;
        timer.restart();
    }
    if (state == "Out" and cur_image >= (images["Out"]->w/rect.w))
        state = "Idle";
    cur_image %= (image->w/rect.w);
}

void Checkpoint::bump(const std::string& flag)
{
    if (state == "Empty")
        state = "Out";
}

Start::Start(int _x, int _y, SDL_Rect* v):
    Checkpoints("Start", _x, _y, v),
    first(true), delay(0)
{
    images["Idle"] = IMG_Load("images/Items/Checkpoints/Start/Start (Idle).png");
    images["Moving"] = IMG_Load("images/Items/Checkpoints/Start/Start (Moving) (64x64).png");
    state = "Idle";
}

void Start::update()
{
    Checkpoints::update();
    if (!first)
        if (state_timer.get_elapsed_s() >= delay)
        {
            state = (state=="Moving")?"Idle":"Moving";
            delay = randint(1, 10);
            state_timer.restart();
        }
}

void Start::bump(const std::string& flag)
{
    if (state == "Idle")
        state = "Moving";
    first = false;
}

End::End(int _x, int _y, SDL_Rect* v):
    Start(_x, _y, v)
{
    type.pop_back();
    type.push_back("End");
    SDL_FreeSurface(images["Idle"]);
    images["Idle"] = IMG_Load("images/Items/Checkpoints/End/End (Idle).png");
    SDL_FreeSurface(images["Moving"]);
    images["Moving"] = IMG_Load("images/Items/Checkpoints/End/End (Pressed) (64x64).png");
}
