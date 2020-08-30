#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <vector>
#include <map>
#include <SDL.h>
#include "object.h"

class GameObject;

/// Classe conteneur des sprites
class Group
{
public:
    virtual ~Group();
    static void clear_buffer();

    /// Ajoute le sprite dans le groupe
    void add(GameObject*);
    void add(std::vector<GameObject*>);

    /// Verifie si le groupe est vide
    bool empty();

    /// Met-à-jour les sprites du groupe
    /// Cette méthode appelle GameObject::update
    virtual void update();

    /// Déssine les sprites du groupe
    /// Cette méthode appelle GameObject::draw
    void draw(SDL_Surface*);

    /// Demande si le groupe contient le sprite
    bool has(GameObject*);
    bool has(std::vector<GameObject*>);

    /// Retire un sprite du groupe
    void remove(GameObject*);
    void remove(std::vector<GameObject*>);

    /// Retire toute sprite de ce groupe
    /// Néanmoins, les pointeurs ne seront pas liberés
    void clear();

    /// Obtenir une liste des sprites du groupe
    std::vector<GameObject*> sprites();

    GameObject* operator[](int);

    /// @return Liste des sprites du groupe qui entrent en collision avec le sprite passé en paramètre
    /// @param bool _kill : on retire les sprites trouvés du groupe si _kill vaut true
    std::vector<GameObject*> sprites_colliding_with(GameObject*, bool _kill = false);

    /// \brief Detect collision between the group and a game object or a rectangular shape or a single point
    /// \return A pointer to the game object that collided with the object given as parameter
    GameObject* first_sprite_colliding_with(GameObject*);
    GameObject* first_sprite_colliding_with(SDL_Rect);
    GameObject* first_sprite_colliding_with(int, int);

    /// @brief Teste la collision entre deux groupes de sprites
    /// @param bool _kill : on retire les sprites trouvés du groupe si ce paramètre vaut true
    /// @param bool __kill: on retire les sprites trouvés du groupe passé en paramètre si __kill vaut true
    /// @return map addméttant les sprites du groupe comme clés et les sprites du groupe passé en paramètre comme valeur
    std::map< GameObject*, std::vector<GameObject*> > collide_with(Group*, bool _kill = false, bool __kill = false);

protected:
    /// Liste des sprites contenus dans le groupe
    std::vector<GameObject*> sprite_list;

private:

    bool _has(GameObject*);
    void _remove(GameObject*);
    void _add(GameObject*);
    GameObject* get(int);
    static std::vector<GameObject*> buffer;
};

#endif
