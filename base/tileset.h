#ifndef TILESET_H
#define TILESET_H

<<<<<<< HEAD
#include <SDL/SDL.h>
#include <string>
=======
#include <string>
#include <SDL.h>
>>>>>>> 495eb918d6ae1f887b19d2afaa37aeac42acb24c
#include <vector>

/**
 * Load images associated with a given tileset name.
 * Also, load tiles informations such as name and id,
 * positions of a given tile in its associated spritesheet
 * from a xml file; otherwise, only image resources will be loaded.
 */
class Tileset
{
public:
    Tileset(const std::string& file_name = "");
    ~Tileset();

    SDL_Surface* surface;
    // Réspectant le repère de coordonnées de la SDL
    int tile_x, tile_y;

private:
    int m_tilew, m_tileh;
};

#endif
