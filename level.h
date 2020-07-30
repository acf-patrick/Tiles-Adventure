#ifndef LEVEL_H
#define LEVEL_H

#include "base/map.h"
#include "base/group.h"
#include "base/func_tool.h"
#include <cmath>
#include <map>
#include <tmx.h>

class RestrictedGroup : public Group
{
public:
    RestrictedGroup(SDL_Rect*);
    void update();

private:
    std::vector<bool> first;
    SDL_Rect* viewport;
};

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

    void center_on(GameObject*, SDL_Rect);

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
            return sprite->get_impulse()[1] > 0;
        }
    };
    void remove_enemy(GameObject*);

    bool earthquake;
    Group enemies, dying;
    Group traps;
    std::map<std::string, Group> items;

    void __load_objects(tmx_layer*);
};

#endif
