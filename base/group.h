#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <vector>
#include <map>
#include <SDL.h>
#include "sprite.h"

class Sprite;

/// Classe conteneur des sprites
class Group
{
public:
    virtual ~Group();
    static void clear_buffer();

    /// Ajoute le sprite dans le groupe
    void add(Sprite*);
    void add(std::vector<Sprite*>);

    /// Verifie si le groupe est vide
    bool empty();

    /// Met-à-jour les sprites du groupe
    /// Cette méthode appelle Sprite::update
    virtual void update();

    /// Déssine les sprites du groupe
    /// Cette méthode appelle Sprite::draw
    void draw(SDL_Surface*);

    /// Demande si le groupe contient le sprite
    bool has(Sprite*);
    bool has(std::vector<Sprite*>);

    /// Retire un sprite du groupe
    void remove(Sprite*);
    void remove(std::vector<Sprite*>);

    /// Retire toute sprite de ce groupe
    /// Néanmoins, les pointeurs ne seront pas liberés
    void clear();

    /// Obtenir une liste des sprites du groupe
    std::vector<Sprite*> sprites();

    Sprite* operator[](int);

    /// @return Liste des sprites du groupe qui entrent en collision avec le sprite passé en paramètre
    /// @param bool _kill : on retire les sprites trouvés du groupe si _kill vaut true
    std::vector<Sprite*> sprites_colliding_with(Sprite*, bool _kill = false);

    /// Retourne le premier sprite trouvé ,qui entre en collision avec le sprite donné en paramètre
    Sprite* first_sprite_colliding_with(Sprite*);
    // Rect
    Sprite* first_sprite_colliding_with(SDL_Rect);
    // Point
    Sprite* first_sprite_colliding_with(int, int);

    /// @brief Teste la collision entre deux groupes de sprites
    /// @param bool _kill : on retire les sprites trouvés du groupe si ce paramètre vaut true
    /// @param bool __kill: on retire les sprites trouvés du groupe passé en paramètre si __kill vaut true
    /// @return map addméttant les sprites du groupe comme clés et les sprites du groupe passé en paramètre comme valeur
    std::map< Sprite*, std::vector<Sprite*> > collide_with(Group*, bool _kill = false, bool __kill = false);

protected:
    /// Liste des sprites contenus dans le groupe
    std::vector<Sprite*> sprite_list;

private:

    bool _has(Sprite*);
    void _remove(Sprite*);
    void _add(Sprite*);
    Sprite* get(int);
    static std::vector<Sprite*> buffer;
};

#endif
