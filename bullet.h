#ifndef BULLET_H
#define BULLET_H

#include "base/with_mass.h"
#include "base/timer.h"
#include <SDL_image.h>
#include "base/map.h"

class Bullet: public With_mass
{
public:
    static bool allow_fire();

    Bullet(const std::string&, Map*, int, int, int);
    void update();
    void bump(const std::string& flag = "");

private:
    static bool first_instance;
    static Timer last_instanciation;
};

class Bullet_Pieces: public Sprite
{
    class Bullet_Piece: public With_mass
    {
    public:
        Bullet_Piece(Map* m, int _x, int _y, int dir, int num, bool b):
            With_mass(m, _x, _y)
        {
            gravity *= 0.5;
            image = IMG_Load("images/Enemies/Trunk/Bullet Pieces.png");
            if (!image)
            {
                std::cerr << IMG_GetError();
                exit(EXIT_FAILURE);
            }
            rect.w = rect.h = sub.w = sub.h = 16;
            sub.x = num*sub.w;
            sub.y = 0;
            if (b)
                x_vel = dir?3:-3;
            else
            {
                if (dir)
                {
                    x -= rect.w;
                    x_vel = -5;
                }
                else
                    x_vel = 5;

                if (num)
                    y_vel = -8;
                else
                    y_vel = -4;
            }
        }

        void update()
        {
            if (!x_vel and timer.get_elapsed_s() >= 3)
                kill();
            apply_gravity();
            move(x_vel, y_vel);

            y += 2;
            if (m_map->collision_with(this))
            {
                x_vel = y_vel = 0;
                if (x_vel)
                    timer.restart();
            }
            y -= 2;
        }

        void draw(SDL_Surface* screen)
        {
            show_boundingBox(screen);
            rect.x = Sint16(x-m_map->get_xshift());
            rect.y = Sint16(y-m_map->get_yshift());
            SDL_BlitSurface(image, &sub, screen, &rect);
        }

        bool collide_with(Sprite* sprite)
        {
            if (!sprite)
                return false;
            int num = sub.x/sub.w;
            SDL_Rect r;
            if (num)
            {
                r.x = Sint16(x+21);
                r.y = Sint16(y+5);
                r.w = 6;
                r.h = 7;
            }
            else
            {
                r.x = Sint16(x+4);
                r.y = Sint16(y+5);
                r.w = 8;
                r.h = 7;
            }
            return rects_collide(r, sprite->get_rect());
        }

    private:
        SDL_Rect sub;
        Timer timer;
    };

public:
    Bullet_Pieces(Map*, int, int, int, bool b = false);
    void update();
    void draw(SDL_Surface*);

private:
    Group pieces;
};

#endif
