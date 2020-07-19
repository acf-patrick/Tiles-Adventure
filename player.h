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
#include "base/object.h"

class Player: public With_mass
{
public:
    Player(Map*, int, int, bool*, const std::string&);
    ~Player();
    void update();
    void draw(SDL_Surface*);
    void bump(const std::string& flag = "");
    bool collide_with(GameObject*);
    SDL_Surface* get_surface();
    float *get_impulse();

private:
    class Dust: public GameObject
    {
    public:
        Dust(SDL_Rect* v, int m_x, int m_y, const std::string& effect):
            cur_image(0), viewport(v)
        {
            int img_nbr = (effect != "Jump")?5:6;
            for (int i=0; i<img_nbr; ++i)
            {
                std::stringstream file_name;
                file_name << "images/Dust Particles/" << effect << " 0";
                file_name << i+1 << ".png";
                images.push_back(IMG_Load(file_name.str().c_str()));
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
            if (timer.get_elapsed_ms() >= 60)
            {
                cur_image++;
                timer.restart();
            }
            if (cur_image >= (int)images.size())
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
        std::vector<SDL_Surface*> images;
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
