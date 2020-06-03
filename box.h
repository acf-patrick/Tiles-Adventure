#ifndef BOX_H
#define BOX_H

#include <map>
#include <SDL.h>
#include <sstream>
#include <SDL_image.h>
#include "base/with_mass.h"
#include "base/func_tool.h"
#include "base/sprite.h"
#include "base/timer.h"
#include "base/map.h"

class Box: public With_mass
{
public:
    Box(Map*, int, int, int);
    void update();
    void draw(SDL_Surface*);
    void bump(const std::string& flag = "");

private:
    int numero;
    int cur_image;
    std::string state;
    Timer timer;
    std::map<std::string, SDL_Surface*> images;
    class BoxPiece: public With_mass
    {
    public:
        enum { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

        BoxPiece(Map* m, int s_x, int s_y, int nbr, int pos):
            With_mass(m, s_x, s_y)
        {
            std::stringstream oss;
            oss << "images/Items/Boxes/Box" << nbr << "/Break.png";
            image = IMG_Load(oss.str().c_str());
            if (!image)
            {
                std::cerr << IMG_GetError();
                exit(EXIT_FAILURE);
            }
            rect.x = pos*28;
            rect.y = 0;
            rect.w = 28;
            rect.h = 24;
            if (pos == BOTTOM_RIGHT or pos == TOP_RIGHT)
            {
                x_vel = 2;
                if (pos == TOP_RIGHT)
                    y_vel = -4;
            }
            else if (pos == BOTTOM_LEFT or pos == TOP_LEFT)
            {
                x_vel = -2;
                if (pos == TOP_LEFT)
                    y_vel = -4;
            }
        }
        void draw(SDL_Surface* screen)
        {
            SDL_Rect pos = { Sint16(x-m_map->get_xshift()), Sint16(y-m_map->get_yshift()) };
            SDL_BlitSurface(image, &rect, screen, &pos);
        }
        bool collide_with(Sprite* sprite)
        {
            if (!sprite)
                return false;
            SDL_Surface* surface = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32,
                                                        image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
            SDL_Surface* s_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sprite->get_rect().w, sprite->get_rect().h,
                                                          32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 100, 100));
            SDL_FillRect(s_surface, NULL, 0x0);
            SDL_BlitSurface(image, &rect, surface, 0);
            int xs(m_map->get_xshift()), ys(m_map->get_yshift());
            bool ret = images_collide(x-xs, y-ys, surface, sprite->get_x()-xs, sprite->get_y()-ys, s_surface);
            SDL_FreeSurface(surface);
            SDL_FreeSurface(s_surface);
            return ret;
        }
    };
};

#endif // BOX_H
