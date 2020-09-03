#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include "base/func_tool.h"
#include "bullet.h"

Timer Bullet::last_instanciation;
bool Bullet::first_instance(true);

bool Bullet::allow_fire()
{
    return first_instance or last_instanciation.get_elapsed_ms() >= 450;
}

Bullet::Bullet(const std::string& enemy, Map* m, int _x, int _y, int dir):
    With_mass(m, _x, _y)
{
    first_instance = false;
    last_instanciation.restart();
    direction = dir;
    type.push_back("Bullet");
    std::string path("images/Enemies/");
    path += enemy + "/Bullet.png";
    if (dir)
        image = IMG_Load(path.c_str());
    else
        image = flip(IMG_Load(path.c_str()), true, false);
    x_vel = 4*(dir?1:-1);
    rect.w = rect.h = 16;

    if (enemy == "Trunk")
    {
        y += 12;
        x += dir?47:1;
    }
    else if (enemy == "Plant")
    {
        y += 11;
        x += dir?35:-7;
    }
}

void Bullet::update()
{
    move(x_vel, 0);
    x += x_vel;
    if (m_map->collision_with(this))
        bump();
    x -= x_vel;
}

void Bullet::bump(const std::string& flag)
{
    groups[0]->add(new Bullet_Pieces(m_map, direction?get_right():x, y, direction, !flag.empty()));
    kill();
}

Bullet_Pieces::Bullet_Pieces(Map* m, int _x, int _y, int dir, bool b)
{
    for (int i=0; i<2; ++i)
        pieces.add(new Bullet_Piece(m, _x, _y, dir, i, b));
}

void Bullet_Pieces::update()
{
    pieces.update();
}

void Bullet_Pieces::draw(SDL_Surface* screen)
{
    pieces.draw(screen);
}
