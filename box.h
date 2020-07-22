#ifndef BOX_H
#define BOX_H

#include <map>
#include <SDL.h>
#include <tmx.h>
#include <sstream>
#include <SDL_image.h>
#include "base/with_mass.h"
#include "base/func_tool.h"
#include "base/creator.h"
#include "base/object.h"
#include "base/timer.h"
#include "base/map.h"

class Box: public With_mass
{
public:
    Box(Map*, int, int, int);
    ~Box();
    void update();
    void draw(SDL_Surface*);
    void bump(const std::string& flag = "");

    class Creator:  public ObjectCreator
    {
    public:
        GameObject* operator()(int _x, int _y)
        {
            tmx_properties* props(((tmx_layer*)getParameter("current layer"))->properties);
            return new Box((Map*)getParameter("game map"), _x, _y, tmx_get_property(props, "numero")->value.integer);
        }
    };
private:
    int numero;
    int cur_image;
    float hp;
    std::string state;
    Timer timer;
    std::map<std::string, SDL_Surface*> images;
    class BoxPiece: public With_mass
    {
    public:
        BoxPiece(Map* m, int s_x, int s_y, int nbr, int pos):
            With_mass(m, s_x, s_y), m_pos(pos)
        {
            type.push_back("Debris");
            gravity *= 0.5;
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
            x -= 0.5*rect.w;
            y -= 0.5*rect.h;
            if (pos == BOTTOM_RIGHT or pos == TOP_RIGHT)
            {
                x_vel = 2;
                y_vel = (pos == TOP_RIGHT)?-6:-2;
            }
            else if (pos == BOTTOM_LEFT or pos == TOP_LEFT)
            {
                x_vel = -2;
                y_vel = (pos == TOP_LEFT)?-6:-2;
            }
            timer.restart();
        }
        void draw(SDL_Surface* screen)
        {
            show_boundingBox(screen);
            SDL_Rect pos = { Sint16(x-m_map->get_xshift()), Sint16(y-m_map->get_yshift()) };
            rect.x = m_pos*rect.w;
            rect.y = 0;
            SDL_BlitSurface(image, &rect, screen, &pos);
        }
        void update()
        {
            apply_gravity();
            move (x_vel, y_vel);
            if (timer.get_elapsed_s() >= 5)
                kill();
            y += 2;
            if (m_map->collision_with(this))
                x_vel = y_vel = 0;
            y -= 2;
        }
        bool collide_with(GameObject* sprite)
        {
            if (!sprite)
                return false;
            if (sprite->is("Player"))
                return GameObject::collide_with(sprite);
            SDL_Surface* surface = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32,
                                                        image->format->Rmask, image->format->Bmask,
                                                        image->format->Bmask, image->format->Amask);
            SDL_Surface* s_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sprite->get_rect().w, sprite->get_rect().h,
                                                          32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 100, 100));
            SDL_FillRect(s_surface, NULL, SDL_MapRGB(s_surface->format, 255, 0, 0));
            rect.x = m_pos*rect.w;
            rect.y = 0;
            SDL_BlitSurface(image, &rect, surface, 0);
            int xs(m_map->get_xshift()), ys(m_map->get_yshift());
            bool ret = images_collide(x-xs, y-ys, surface, sprite->get_x()-xs, sprite->get_y()-ys, s_surface);
            SDL_FreeSurface(surface);
            SDL_FreeSurface(s_surface);
            return ret;
        }
    private:
        enum { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

        int m_pos;
        Timer timer;
    };
};

#endif // BOX_H
