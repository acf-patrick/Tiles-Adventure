#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <sstream>
#include <SDL_image.h>
#include "show.h"
#include "base/with_mass.h"
#include "base/map.h"
#include "base/timer.h"
#include "base/sprite.h"

class Player: public With_mass
{
public:
    Player(Map*, int, int, bool*, const std::string&);
    ~Player();
    void update();
    void draw(SDL_Surface*);
    void bump(const std::string& flag = "");
    bool collide_with(Sprite*);
    SDL_Surface* get_surface();

private:
    class Dust: public Sprite
    {
    public:
        Dust(SDL_Rect* v, int m_x, int m_y):
            cur_image(0), viewport(v)
        {
            for (int i=0; i<6; ++i)
            {
                std::stringstream file_name;
                file_name << "images/Dust Particles/Jump 0";
                file_name << i+1 << ".png";
                images[i] = IMG_Load(file_name.str().c_str());
                if (!images[i])
                {
                    std::cerr << IMG_GetError();
                    exit(EXIT_FAILURE);
                }
            }
            x = m_x-0.5*images[0]->w;
            y = m_y-0.5*images[0]->h;
            timer.restart();
        }
        void update()
        {
            image = images[cur_image];
            if (timer.get_elapsed_ms() >= 50)
            {
                cur_image++;
                timer.restart();
            }
            if (cur_image >= 6)
                kill();
        }
        void draw(SDL_Surface* screen)
        {
            rect.x = Sint16(x-viewport->x);
            rect.y = Sint16(y-viewport->y);
            SDL_BlitSurface(image, NULL, screen, &rect);
        }
    private:
        int cur_image;
        SDL_Rect *viewport;
        Timer timer;
        SDL_Surface* images[6];
    };
    Show* show;
    std::map<std::string, SDL_Surface*> images[2];
    bool* keys;
    std::string state;
    int cur_image;
    Timer timer;
    bool allow_jump;
    bool double_jump;
    bool first_impulse;
    const float JUMP_GRAVITY;
    const float JUMP_IMPULSE;
    const float MAX_FALL_SPEED;
    Group dust;

    void respawn();
    void jump();
    void update_images();
    void update_states();
    void update_keys();
    void update_xvel();
    void update_yvel();
};

#endif // PLAYER_H
