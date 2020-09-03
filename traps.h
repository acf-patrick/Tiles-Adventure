#ifndef TRAPS_H
#define TRAPS_H

#include <SDL/SDL.h>
#include "bubbles.h"
#include "base/creator.h"
#include "base/object.h"
#include "base/timer.h"

class Arrow: public GameObject
{
public:
    Arrow(SDL_Rect*, int, int);
    ~Arrow();
    void update();
    void bump(const std::string& flag = "");
    void draw(SDL_Surface*);

    static SDL_Rect* create_static_viewport();

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Arrow((SDL_Rect*)getParameter("game viewport"), _x, _y);
        }
    };

private:
    SDL_Rect* viewport;
    bool used;
    int cur_image;
    SDL_Surface *hit, *idle;
    Timer timer;

};

class Basic_fan: public GameObject
{
public:
    Basic_fan(SDL_Rect*, int, int);
    ~Basic_fan();
    virtual void update();
    virtual void draw(SDL_Surface*);

    static void draw_bubbles(SDL_Surface*);
    static void update_bubbles(Map*);
    static void destroy_bubbles();

protected:
    SDL_Rect* viewport;
    bool off;
    int sens;
    int cur_image;
    // hauteur des bulles
    int min_tar, max_tar;
    SDL_Surface *s_off, *s_on;
    Timer timer;

    int animation_delay;
    bool extinction, demarrage;

    // L'objet servira d'icone dans l'editeur de niveau
    bool m_static;

private:
    static Group *bubbles;
};

class Falling_platform: public Basic_fan
{
public:
    Falling_platform(SDL_Rect*, int, int);
    void update();
    void bump(const std::string& flag = "");
    bool collide_with(GameObject*);

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Falling_platform((SDL_Rect*)getParameter("game viewport"), _x, _y);
        }
    };
private:
    int y0;
    float y_vel, gravity;

    void turn_off();
};

class Fan: public Basic_fan
{
public:
    Fan(SDL_Rect*, int, int, GameObject*, int);
    void update();
    void draw(SDL_Surface*);

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Fan((SDL_Rect*)getParameter("game viewport"), _x, _y, (GameObject*)getParameter("Player"), ((tmx_object*)getParameter("current object"))->rotation);
        }
    };
private:
    Chrono switch_timer;
    GameObject* target;
    SDL_Rect field;
};

class Spike: public GameObject
{
public:
    Spike(SDL_Rect*, int, int, int);
    void draw(SDL_Surface*);
    SDL_Surface* get_surface();
    bool collide_with(GameObject*);

    class Creator : public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            return new Spike((SDL_Rect*)getParameter("game viewport"), _x, _y, ((tmx_object*)getParameter("current object"))->rotation);
        }
    };

private:
    SDL_Rect* viewport;
};

#endif // TRAPS_H
