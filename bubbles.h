#ifndef BUBBLES_H
#define BUBBLES_H

#include <SDL.h>
#include "base/sprite.h"

/** Bulles crées lorsque les ventilos se mettent en marche */
class Bubbles: public Sprite
{
public:
    enum { HAUT, DROITE, BAS, GAUCHE };
    /**
    * @param v : viewport de la map
    * @param fan_rect : boite englobante de l'objet Basic_fan
    * @param sens : sens de propagation des bulles
    * @param min_tar : distance minimale que la bulle doit parcourir
    * @param max_tar : distance maximale que la bulle peut parcourir
    */
    Bubbles(SDL_Rect*, SDL_Rect, int sens, int min_tar, int max_tar);
    void update();
    void draw(SDL_Surface*);

private:
    int m_sens;
    SDL_Rect* viewport;
    float x_vel, y_vel, acc;

    // la bulle rebondit quand elle atteint
    // la position indiquée par ces paramètres
    int l_bound, u_bound;

    int target;
    bool cinematic;
};

#endif // BUBBLES_H
