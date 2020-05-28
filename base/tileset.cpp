#include "tileset.h"
#include "func_tool.h"
#include <SDL_image.h>
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
        name = file_name;
        name += ".xml";
        std::ifstream xml(name.c_str());
        if (xml)
        {
            std::string buffer;
            bool first(true);
            while (getline(xml, buffer))
            {
                if (first or buffer[1] == '/')
                {
                    first = false;
                    continue;
                }
                buffer = replace(buffer, "<SubTexture name=", " ");
                buffer = replace(buffer, "/>", " ");
                buffer = replace(buffer, ".png", "");
                buffer = strip(buffer);
                std::vector<std::string> ligne(split(buffer, "\""));
                std::stringstream *flux;
                flux = new std::stringstream(ligne[8]);
                *flux >> m_tileh;
                flux = new std::stringstream(ligne[6]);
                *flux >> m_tilew;
                flux = new std::stringstream(ligne[4]);
                int x, y;
                *flux >> x;
                flux = new std::stringstream(ligne[2]);
                *flux >> y;
                delete flux;
                x /= m_tilew;
                y /= m_tileh;
                if (m_names.empty())
                {
                    tile_x = surface->w/32;
                    tile_y = surface->h/32;
                    m_names.resize((tile_x*tile_y), "blank");
                }
                int index(x*tile_x+y);
                m_names[index] = ligne[0];
            }
            xml.close();
        }
        else
            std::cerr << name << " file not found while loading tileset!\n";
    }
}
Tileset::~Tileset()
{
    SDL_FreeSurface(surface);
    surface = NULL;
}

SDL_Rect* Tileset::get(const std::string& name)
{
    if (m_names.empty())
        return NULL;

    int index(0);
    while (m_names[index] != name and index < (int)m_names.size())
        index++;

    SDL_Rect* ret(NULL);
    if (index == (int)m_names.size())
        return ret;
    ret = new SDL_Rect;
    ret->x = m_tilew*(index%tile_x);
    ret->y = m_tileh*(index/tile_x);
    ret->w = m_tilew;
    ret->h = m_tileh;
    return ret;
}

SDL_Rect* Tileset::get(int index)
{
    SDL_Rect* ret(NULL);
    if (index < 0 or index >= (int)m_names.size())
        return ret;
    ret = new SDL_Rect;
    ret->x = m_tilew*(index%tile_x);
    ret->y = m_tileh*(index/tile_x);
    ret->w = m_tilew;
    ret->h = m_tileh;
    return ret;
}
