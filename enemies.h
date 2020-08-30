#ifndef ENEMIES_H
#define ENEMIES_H

#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include "base/creator.h"
#include "base/func_tool.h"
#include "player.h"
#include "enemy.h"

class AngryPig: public Enemy
{
public:
    AngryPig(Map*, int, int);
    void bump(const std::string& flag = "");
    bool collide_with(GameObject*);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new AngryPig((Map*)getParameter("game map"), _x, _y);
        }
    };
};

class Bunny : public Enemy
{
public:
    Bunny(Map*, int, int);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Bunny((Map*)getParameter("game map"), _x, _y);
        }
    };
};

class Chicken : public Enemy
{
public:
    Chicken(Map*, int, int);
    virtual void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Chicken((Map*)getParameter("game map"), _x, _y);
        }
    };
};

class Chameleon: public Enemy
{
public:
    Chameleon(Map*, int, int, GameObject*);
    void update();
    void draw(SDL_Surface*);
    SDL_Surface* get_surface();
    void bump(const std::string&);
    bool collide_with(GameObject*);

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Chameleon((Map*)getParameter("game map"), _x, _y, (GameObject*)getParameter("Player"));
        }
    };
private:
    GameObject* target;
    SDL_Rect img_rect;
};

class Duck: public Enemy
{
public:
    Duck(Map*, int, int);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Duck((Map*)getParameter("game map"), _x, _y);
        }
    };
};

class Ghost: public Enemy
{
public:
    Ghost(Map*, int, int);
    void draw(SDL_Surface*);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Ghost((Map*)getParameter("game map"), _x, _y);
        }
    };
private:
    bool visible;
};

class Mushroom: public Enemy
{
public:
    Mushroom(Map*, int, int);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Mushroom((Map*)getParameter("game map"), _x, _y);
        }
    };
};

class Plant: public Shooter
{
public:
    Plant(Map*, int, int, GameObject*, Group*);
    void update();

    class Creator: public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Plant((Map*)getParameter("game map"),
                              _x, _y,
                              (GameObject*)getParameter("Player"),
                             (Group*)getParameter("bullets group"));
        }
    };
};

class Rino: public Enemy
{
public:
    Rino(Map* ,int, int, GameObject*);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Rino((Map*)getParameter("game map"), _x, _y, (GameObject*)getParameter("Player"));
        }
    };
private:
    bool charge;
    GameObject* target;
};

class Slime: public Chicken
{
public:
    Slime(Map*, int, int);
    void update();
    SDL_Surface* get_surface();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Slime((Map*)getParameter("game map"), _x, _y);
        }
    };
protected:
    void update_image();
};

class Skull: public Enemy
{
    class Particle: public GameObject
    {
    public:
        Particle(int source_x, int source_y, SDL_Rect* v):
            x_vel(1), y_vel(-1), viewport(v)
        {
            x = x_tar = randint(source_x-26, source_x+26);
            y = randint(source_y-27, source_y+27);
            y_tar = y-randint(40, 100);
            std::string name("images/Enemies/Skull/");
            name += ((rand()%2)?"Red":"Orange");
            name += " Particle.png";
            float zoom(10.0/(randint(10, 20)));
            image = zoomSurface(IMG_Load(name.c_str()), zoom, zoom, true);
            if (!image)
            {
                std::cerr << IMG_GetError();
                exit(EXIT_FAILURE);
            }
        }
        void update()
        {
            if (y <= y_tar)
                kill();
            if (x_vel<0 and x<x_tar)
            {
                x_tar = x+randint(0, 50);
                x_vel *= -1;
            }
            else if (x_vel>0 and x>x_tar)
            {
                x_tar = randint(x-50, x);
                x_vel *= -1;
            }
            y += y_vel;
            x += x_vel;
        }
        void draw(SDL_Surface* screen)
        {
            rect.x = Sint16(x-viewport->x);
            rect.y = Sint16(y-viewport->y);
            SDL_BlitSurface(image, NULL, screen, &rect);
        }

    private:
        float x_vel, y_vel;
        SDL_Rect* viewport;
        int x_tar, y_tar;
    };

public:
    Skull(Map*, int, int, GameObject*);
    void update();
    void draw(SDL_Surface*);

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Skull((Map*)getParameter("game map"), _x, _y, (GameObject*)getParameter("Player"));
        }
    };

private:
    bool charge;
    GameObject* target;
    SDL_Rect field;
    Group particles;
    Timer particle_timer;
};

class Trunk: public Shooter
{
public:
    Trunk(Map*, int, int, GameObject*, Group*);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Trunk((Map*)getParameter("game map"), _x, _y, (GameObject*)getParameter("Player"), (Group*)getParameter("bullets group"));
        }
    };
};

class Turtle: public Enemy
{
public:
    Turtle(Map*, int, int);
    void update();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Turtle((Map*)getParameter("game map"), _x, _y);
        }
    };
};

#endif
