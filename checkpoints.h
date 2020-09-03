#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H

#include "base/object.h"
#include "base/creator.h"
#include "base/timer.h"
#include <SDL/SDL.h>
#include <vector>
#include <map>

class Checkpoints : public GameObject
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

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Checkpoint(_x, _y, (SDL_Rect*)getParameter("game viewport"));
        }
    };
};

class Start : public Checkpoints
{
public:
    Start(int, int, SDL_Rect*);
    void update();
    void bump(const std::string& flag = "");

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Start(_x, _y, (SDL_Rect*)getParameter("game viewport"));
        }
    };
private:
    bool first;
    int delay;
};

class End : public Start
{
public:
    End(int, int, SDL_Rect*);
    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new End(_x, _y, (SDL_Rect*)getParameter("game viewport"));
        }
    };
};

#endif // CHECKPOINTS_H
