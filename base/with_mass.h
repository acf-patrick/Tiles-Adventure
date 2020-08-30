#ifndef WITH_MASS_H
#define WITH_MASS_H

#include <SDL.h>
#include "object.h"
#include "map.h"

/// Classe de base pour les objets soumis à une gravité
class With_mass: public GameObject
{
public:
    static bool showBoundingBox;
    With_mass(Map*, int x0, int y0);
    virtual void draw(SDL_Surface* screen);

    float x_vel, y_vel;
    void set_map(Map*);

protected:
    Map* m_map;
    float gravity;
    bool direction;
    const float GRAVITY;

    enum { LEFT, RIGHT };

    void show_boundingBox(SDL_Surface*);
    void move(int, int);
    bool try_move(int, int);
    void affine(int, int);
    void apply_gravity();

};

#endif
