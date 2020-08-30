#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <map>
#include <SDL.h>
#include "group.h"

class Group;

/// Classe de base pour les objets du jeu
class GameObject
{
public:

    GameObject(SDL_Surface* surface = NULL);
    GameObject(SDL_Rect);
    virtual ~GameObject();

    /// Le sprite appartient au moins à un groupe si retourne true
    bool alive();

    /// Enleve le sprite de tous les groupes qui l'a contenu
    void kill();

    /// Test si les sprites entrent en collision
    /// @see rects_collide
    /// @see circle_n_rect_collide
    virtual bool collide_with(GameObject*);
    bool collide_with(SDL_Rect);
    bool collide_with(int, int);

    /// mettre à jour
    virtual void update();

    /// Déssine le sprite grace aux attributs:
    /// image et rect
    virtual void draw(SDL_Surface*);

    /// Equivalent à :
    /// group->add(sprite)
    void remember(Group*);

    int get_x();
    int get_y();
    int get_right();
    int get_bottom();
    int get_centerx();
    int get_centery();
    SDL_Rect get_rect();
    /// Retourne la force qu'éxerce ce sprite sur un autre
    /// sous forme de tableau à deux cases :
    /// suivant 'x' et suivant 'y'
    virtual float *get_impulse();
    virtual SDL_Surface* get_surface();

    void set_x(Sint16);
    void set_y(Sint16);
    void set_right(Sint16);
    void set_bottom(Sint16);
    void set_center(Sint16, Sint16);
    virtual void set_position(int, int);
    virtual void move(int, int);

    // Utilisée pour les coinBox ou les boites à items
    virtual void bump(const std::string& flag = "");
    // ...
    virtual bool is_out();

    // Géstion des types
    std::string get_type();
    std::string get_ancestor();
    bool is(const std::string&);

    static void check_image_existence(SDL_Surface*);

    // attribut utilisé pour faciliter la géstion des collisions
    bool check_horizontal;
    bool check_up;
    bool check_down;
protected:
    std::vector<std::string> type;

    /// Liste des groupes auxquels le sprite appartient
    std::vector<Group*> groups;

    SDL_Surface* image;
    SDL_Rect rect;
    float x, y;

private:
    /// Insère le sprite dans un groupe
    void add(Group*);
friend class Group;
};

#endif
