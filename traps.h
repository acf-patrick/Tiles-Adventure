#ifndef TRAPS_H
#define TRAPS_H

#include <SDL.h>
#include "bubbles.h"
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

    static void check_image_existence(SDL_Surface*);
    static SDL_Rect* create_static_viewport();

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

private:
    int y0;
    float y_vel, gravity;

    void turn_off();
};

class Fan: public Basic_fan
{
public:
    /// \param s : indique la direction où le ventilateur va souffler
    Fan(SDL_Rect*, int, int, GameObject*, int s = Bubbles::DROITE);
    void update();
    void draw(SDL_Surface*);

private:
    Chrono switch_timer;
    GameObject* target;
    SDL_Rect field;
};


#endif // TRAPS_H
