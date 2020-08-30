#include "tileset.h"
#include "func_tool.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>

Tileset::Tileset(const std::string& file_name)
{
    if (file_name.empty())
    {
        surface = NULL;
        tile_x = tile_y = 0;
    }
    else
    {
        std::string name(file_name);
        name += ".png";
        surface = IMG_Load(name.c_str());
        if (!surface)
        {
            std::cerr << IMG_GetError();
            exit(EXIT_FAILURE);
        }
    }
}
Tileset::~Tileset()
{
    SDL_FreeSurface(surface);
    surface = NULL;
}

SDL_Rect* Tileset::get(int index)
{
    SDL_Rect* ret(NULL);
    if (index < 0 or index >= tile_x*tile_y)
        return ret;
    ret = new SDL_Rect;
    ret->x = m_tilew*(index%tile_x);
    ret->y = m_tileh*(index/tile_x);
    ret->w = m_tilew;
    ret->h = m_tileh;
    return ret;
}
