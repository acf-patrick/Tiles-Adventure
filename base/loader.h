#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <vector>
#include <SDL.h>

class Loader
{
public:
    Loader(const std::string&);

protected:
    // nbr tiles
    // R�spectant le rep�re de coordonn�es de la SDL
    int world_x, world_y;

    // size of tiles
    Uint16 tile_w, tile_h;

    std::vector<std::string> m_tileset;
    std::vector< std::vector<int> > m_map;
    std::vector< std::vector< std::vector<int> > > layers;

private:
    std::vector< std::vector<int> > m_layers;
};

#endif // LOADER_H
