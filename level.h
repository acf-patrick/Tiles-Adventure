#ifndef LEVEL_H
#define LEVEL_H

#include "base/map.h"
#include "base/group.h"
#include <cmath>
#include <map>

class Level: public Map
{
public:
    Level();
    ~Level();

    /// Pour inserer les objets dans le jeu
    /// A enlever quand la conception de cette
    /// classe sera terminée.
    void add_enemies(Sprite*);
    void add_item(Sprite*);
    void add_traps(Sprite*);

    void delete_sprite_at(int, int);
    void draw(SDL_Surface*);
    void update();
    bool collision_with(Sprite*);

    Group bullets;
private:
    class Poutre: public Sprite
    {
    public:
        Poutre(int _x, int _y)
        {
            x = _x;
            y = _y;
            rect.w = rect.h = 16;
        }
        bool collide_with(Sprite* sprite)
        {
            if (!Sprite::collide_with(sprite))
                return false;
            return (std::abs(y-sprite->get_y()) <= 10 and
                sprite->check_down);
        }
    };
    void remove_enemy(Sprite*);

    Group enemies, dying;
    Group traps;
    std::map<std::string, Group> items;

    std::map< std::string, std::vector<SDL_Rect> > enemies_pos;
    std::map< std::string, std::vector<SDL_Rect> > traps_pos;
    std::map< std::string, std::map< std::string, std::vector<SDL_Rect> > > items_pos;
};

#endif
