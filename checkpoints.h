#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H

#include "base/sprite.h"
#include "base/timer.h"
#include <vector>
#include <SDL.h>
#include <map>

class Checkpoints : public Sprite
{
public:
    Checkpoints(const std::string&, int, int, SDL_Rect*);
    ~Checkpoints();

    void draw(SDL_Surface*);
    void update();

protected:
    std::map<std::string, SDL_Surface*> images;
    Timer timer;
    SDL_Rect* viewport;
    int cur_image;
    std::string state;
    Timer state_timer;

};

class Checkpoint : public Checkpoints
{
public:
    Checkpoint(int, int, SDL_Rect*);
    void update();
    void bump(const std::string& flag = "");
};

class Start : public Checkpoints
{
public:
    Start(int, int, SDL_Rect*);
    void update();
    void bump(const std::string& flag = "");

private:
    bool first;
    int delay;
};

class End : public Start
{
public:
    End(int, int, SDL_Rect*);
};

#endif // CHECKPOINTS_H
