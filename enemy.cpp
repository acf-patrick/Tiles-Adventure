#include "base/func_tool.h"
#include <SDL_image.h>
#include <sstream>
#include "bullet.h"
#include "enemy.h"

Enemy::Enemy(Map* m, int _x, int _y, const std::string& name, int w, int h):
    With_mass(m, _x, _y)
{
    std::vector<std::string> key = { "Idle", "Run", "Hit" };
    cur_image = 0;
    delay = 3;
    state = "Idle";
    type.push_back("Enemy");
    type.push_back(name);
    rect.w = w;
    rect.h = h;
    load_images(key);
}
Enemy::~Enemy()
{
    for (int i=0; i<2; ++i)
        for (std::map<std::string, SDL_Surface*>::iterator it=images[i].begin();
            it != images[i].end(); ++it)
        {
            SDL_FreeSurface(it->second);
            images[i][it->first] = NULL;
        }
    image = NULL;
}

void Enemy::load_images(const std::vector<std::string>& key)
{
    for (int i=0; i<(int)key.size(); ++i)
    {
        std::string k(key[i]);
        std::stringstream ss;
        ss << "images/Enemies/" << get_type() <<  '/' << key[i] << " (" << rect.w << 'x' << rect.h << ").png";
        images[0][key[i]] = IMG_Load(ss.str().c_str());
        if (!images[0][key[i]])
        {
            std::cerr << IMG_GetError() << std::endl;
            images[0].erase(key[i]);
            continue;
        }
        images[1][key[i]] = flip(images[0][key[i]], true, false);
    }
}

void Enemy::draw(SDL_Surface* screen)
{
    show_boundingBox(screen);
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = {Sint16(x - m_map->get_xshift()), Sint16(y - m_map->get_yshift())};
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Enemy::bump(const std::string& flag)
{
    if (state != "Hit")
    {
        state = "Hit";
        cur_image = 0;
        timer.restart();
    }
}

void Enemy::wait()
{
    if (state_timer.get_elapsed_s() >= delay)
    {
        state = (state == "Idle")?"Run":"Idle";
        if (state == "Run")
            delay = randint(5, 10);
        else
        {
            delay = randint(1, 3);
            direction = rand()%2;
        }
        state_timer.restart();
    }
}

void Enemy::update_image()
{
    image = images[direction][state];
    if (timer.get_elapsed_ms() >= 50)
    {
        cur_image++;
        timer.restart();
    }
}

void Enemy::stop_falling()
{
    y += 2;
    if (m_map->collision_with(this))
        y_vel = 0;
    y -= 2;
}

bool Enemy::erase()
{
    if (state == "Hit")
    {
        if (cur_image >= image->w/rect.w)
            kill();
        return true;
    }
    return false;
}

SDL_Surface* Enemy::get_surface()
{
    image = images[direction][state];
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32,
                        image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
    SDL_FillRect(ret, NULL, SDL_MapRGB(ret->format, 100, 100, 100));
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_BlitSurface(image, &rect, ret, 0);
    return ret;
}

Shooter::Shooter(const std::string& name, Map* m, int _x, int _y,
           int w, int h, GameObject* cible, Group* bullets_group):
    Enemy(m, _x, _y, name, w, h),
    target(cible), bullets(bullets_group)
{
    load_images(std::vector<std::string>(1, "Attack"));
}

void Shooter::fire()
{
    if (Bullet::allow_fire())
        bullets->add(new Bullet(get_type(), m_map, x, y, direction));
}

void Shooter::draw(SDL_Surface* screen)
{
    Enemy::draw(screen);
    bullets->draw(screen);
}
