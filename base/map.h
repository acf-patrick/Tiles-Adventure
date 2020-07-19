#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include "loader.h"
#include "object.h"
#include "func_tool.h"
#include "tileset.h"

/*
        === MAP FILE STRUCTURE ===
    
    // basic informations
    [header]
    width=
    height=
    tilewidth=
    tileheight

    // tilesets informations
    [tilesets]
    tileset="path to the image ressource","tiles width","tiles height",0,0
    .
    .
    .

    // layers
    [layer]
    type="name of the current layer"
    data="a very long list of numbers"

    // another layer
    [layer]
    type=
    data=
    .
    .
    .

    // and so on... 
*/

/**
  \brief This class implements many of a game map functionalities
 */
class Map: public Loader
{
public:
    /// Creer la carte � partir des fichiers de configuration et de la carte du jeu
    Map(const std::string&, SDL_Rect* viewport_rect = NULL);
    virtual ~Map();

    /// Affiche la carte du jeu
    virtual void draw(SDL_Surface*, bool layer_by_layer = true);
    virtual void draw_decor(SDL_Surface*);

    /// Met-�-jour la carte du jeu � chaque frame
    virtual void update();

    /// Modifier la taille des tiles
    void scale(float);

    /// Fait bouger la camera
    void move(int, int);

    /// D�t�ction des collisions
    virtual bool collision_with(GameObject*);
    virtual bool groups_collide_with(GameObject*);

    // Acc�sseurs
    int get_xshift();
    int get_yshift();
    int get_tile_w();
    int get_tile_h();
    SDL_Rect* get_viewport();
    SDL_Surface* get_spritesheet(const std::string&);

    /// \param coordonn�es du tile DANS LE MAP
    /// \param layer_index : choisi 'm_map' si non sp�cifi�
    std::string get_tile(int, int, int layer_index = -1);
    int get_tile_nbr(int, int, int layer_index = -1);


    /// obtenir les coordonn�es du tile en qu�stion dans sa spritesheet associ�e
    /// \return (-1, -1) si le nom du tile n'y figure pas
    std::pair<int, int> get_tile_coordinate(const std::string& );

    int get_tile_index(const std::string&);

    /// recharge les spritesheet
    /// \param indice du spritesheet, recharge tout si n�gatif
    void reset_spritesheet(int index = -1);

    void center_on(GameObject*);
    void center_on(GameObject*, SDL_Rect);
    void set(int, int, int);

    static SDL_Rect camera;

protected:
    void draw_decor(SDL_Surface*, const std::vector< std::vector<int> >&);
    /// On d�fini dedans si le sprite n'interagit pas
    /// avec les personnages
    virtual bool is_decor(const std::string&);

    /// M�thode utilis�e dans la m�thode ''colision_with''
    /// pour d�finir que faire avec les sprites du map
    virtual bool sprites_collide(GameObject*, GameObject*);

    /// Corrige les coordonn�es de la cam�ra
    void clamp_shift_coords();

    /// Supprime le tile corr�spondant
    void clear(int, int);

    SDL_Rect* viewport;
    std::vector<Tileset*> tileset;
    // utilis� lors de l'appel de la m�thode scale
    // pour avoir une image plus nette des tiles
    std::vector<SDL_Surface*> original_spritesheet;

    // Attribut faite pour les sprites statiques.
    // Pour des sprites anim�es ,creer une groupe
    // g�rant le type de sprite en utilisant la
    // classe de base Group
    std::vector< std::vector<GameObject*> > sprites;

    // cette attribut facilitera le reperage entre chaque spritesheet
    std::vector< std::pair<int, int> > intervals;
    // noms des tuiles
    // par d�faut les cases vides du spritesheet s'appeleront 'blank'
    std::vector<std::string> names;

private:
    void draw_layer(SDL_Surface*, std::vector< std::vector<int> > layer, bool drawing_decor = false);
    void draw_sprites(SDL_Surface*);
    void init_tilesets();

};

#endif
