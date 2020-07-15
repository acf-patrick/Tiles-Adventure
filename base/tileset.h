#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <SDL.h>
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

    SDL_Rect* get(const std::string&);
    SDL_Rect* get(int);

    SDL_Surface* surface;
    // Réspectant le repère de coordonnées de la SDL
    int tile_x, tile_y;
    // Nom de chaque tile
    std::vector<std::string> m_names;

private:
    int m_tilew, m_tileh;
};

#endif
