#ifndef ENEMIES_H
#define ENEMIES_H

#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include "enemy.h"

class AngryPig: public Enemy
{
public:
    AngryPig(Map*, int, int);
    void bump(const std::string& flag = "");
    bool collide_with(Sprite*);
    void update();
};

class Bunny : public Enemy
{
public:
    Bunny(Map*, int, int);
    void update();
};

class Chicken : public Enemy
{
public:
    Chicken(Map*, int, int);
    void update();
};

class Chameleon: public Enemy
{
public:
    Chameleon(Map*, int, int, Sprite*);
    void update();
    void draw(SDL_Surface*);
    SDL_Surface* get_surface();
    bool collide_with(Sprite*);

private:
    Sprite* target;
    SDL_Rect img_rect;
};

class Duck: public Enemy
{
public:
    Duck(Map*, int, int);
    void update();
};

class Mushroom: public Enemy
{
public:
    Mushroom(Map*, int, int);
    void update();
};

class Plant: public Shot
{
public:
    Plant(Map*, int, int, Sprite*, Group*);
    void update();

};

class Rino: public Enemy
{
public:
    Rino(Map* ,int, int, Sprite*);
    void update();

private:
    bool charge;
    Sprite* target;
};

class Skull: public Enemy
{
    class Particle: public Sprite
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
    Skull(Map*, int, int, Sprite*);
    void update();
    void draw(SDL_Surface*);

private:
    bool charge;
    Sprite* target;
    SDL_Rect field;
    Group particles;
    Timer particle_timer;
};

class Trunk: public Shot
{
public:
    Trunk(Map*, int, int, Sprite*, Group*);
    void update();
};

class Turtle: public Enemy
{
public:
    Turtle(Map*, int, int);
    void update();
};

#endif
