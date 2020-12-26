#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include "tileset.h"
#include "func_tool.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
