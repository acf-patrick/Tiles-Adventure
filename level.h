#ifndef LEVEL_H
#define LEVEL_H

#include "base/map.h"
#include "base/group.h"
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
    void remove_enemy(Sprite*);

    Group enemies, dying;
    Group traps;
    std::map<std::string, Group> items;

    std::map< std::string, std::vector<SDL_Rect> > enemies_pos;
    std::map< std::string, std::vector<SDL_Rect> > traps_pos;
    std::map< std::string, std::map< std::string, std::vector<SDL_Rect> > > items_pos;
};

#endif
