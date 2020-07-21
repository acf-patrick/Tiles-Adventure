#ifndef LEVEL_H
#define LEVEL_H

#include "base/map.h"
#include "base/group.h"
#include <cmath>
#include <map>
#include <tmx.h>

class Level: public Map
{
public:
    Level();
    ~Level();

    /// Pour inserer les objets dans le jeu
    /// A enlever quand la conception de cette
    /// classe sera terminée.
    void add_enemies(GameObject*);
    void add_item(GameObject*);
    void add_traps(GameObject*);

    void delete_sprite_at(int, int);
    void draw(SDL_Surface*);
    void update();
    bool collision_with(GameObject*);

    void load_objects();

    Group bullets;

private:
    void setup_creators();

    class Poutre: public GameObject
    {
    public:
        Poutre(int _x, int _y)
        {
            x = _x;
            y = _y;
            rect.w = rect.h = 16;
        }
        bool collide_with(GameObject* sprite)
        {
            if (!GameObject::collide_with(sprite))
                return false;
            return (std::abs(y-sprite->get_y()) <= 10 and
                sprite->check_down);
        }
    };
    void remove_enemy(GameObject*);

    Group enemies, dying;
    Group traps;
    std::map<std::string, Group> items;

    void __load_objects(tmx_layer*);
};

#endif
